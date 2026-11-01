# Inject filesystem/signature operations; never change a real Windows ACL.
$ErrorActionPreference = 'Stop'
# Load built-in functions before injecting replacements; module autoload must
# not replace a fake halfway through a case.
Import-Module Microsoft.PowerShell.Utility, Microsoft.PowerShell.Security, Microsoft.PowerShell.Management
$repair = Join-Path (Split-Path -Parent $PSScriptRoot) 'tools\repair-windows-midi-catalog.ps1'
$catalog = Join-Path $env:SystemRoot 'System32\CatRoot\{F750E6C3-38EE-11D1-85E5-00C04FC295EE}\Microsoft-Windows-Client-Desktop-Required-Package~31bf3856ad364e35~amd64~~10.0.26100.9711.cat'
$original = 'O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)'
$serviceSid = ([Security.Principal.NTAccount]::new('NT SERVICE\MidiSrv')).Translate([Security.Principal.SecurityIdentifier]).Value

function Assert-True($condition, $message) { if (-not $condition) { throw $message } }
function Get-AuthenticodeSignature {
    param($LiteralPath)
    [PSCustomObject]@{Status=$global:C100CatalogTest.Status;SignerCertificate=[PSCustomObject]@{Subject='CN=Microsoft Windows, O=Microsoft Corporation'}}
}
function Get-FileHash { param($LiteralPath,$Algorithm) [PSCustomObject]@{Hash=('A' * 64)} }
function Get-Acl { param($LiteralPath) $global:C100CatalogTest.Acl }
function New-Item { param($ItemType,$Path,[switch]$Force) }
function Set-Content {
    param($LiteralPath,$Encoding,[Parameter(ValueFromPipeline=$true)]$Value)
    process {
        $global:C100CatalogTest.Calls.Add('backup')
        $global:C100CatalogTest.Backup = $Value | ConvertFrom-Json
    }
}
function Set-Acl {
    param($LiteralPath,$AclObject)
    $global:C100CatalogTest.Calls.Add('set-acl')
    $global:C100CatalogTest.WrittenPath = $LiteralPath
    $global:C100CatalogTest.WrittenAcl = $AclObject
}

$cases = @(
    @{Name='read-only inspection';Apply=$false;Sddl=$original;Status='Valid';Changes=0},
    @{Name='repair recognized fault';Apply=$true;Sddl=$original;Status='Valid';Changes=1},
    @{Name='already repaired';Apply=$true;Sddl=($original + "(A;;FR;;;$serviceSid)");Status='Valid';Changes=0},
    @{Name='reject unsigned catalog';Apply=$true;Sddl=$original;Status='NotSigned';Error=$true},
    @{Name='reject unrelated ACL';Apply=$true;Sddl=($original + '(A;;FR;;;BU)');Status='Valid';Error=$true},
    @{Name='reject unrelated path';Apply=$true;Sddl=$original;Status='Valid';Error=$true;Path=(Join-Path $PSScriptRoot 'not-a-catalog.cat')}
)
try {
    foreach ($case in $cases) {
        $acl = [Security.AccessControl.FileSecurity]::new()
        $acl.SetSecurityDescriptorSddlForm($case.Sddl)
        $global:C100CatalogTest = @{Acl=$acl;Status=$case.Status;Calls=[Collections.Generic.List[string]]::new()}
        $path = if ($case.Path) { $case.Path } else { $catalog }
        $failed = $false
        try {
            $output = & $repair -CatalogPath $path -Apply:$case.Apply
            $result = ($output -join [Environment]::NewLine) | ConvertFrom-Json
        } catch {
            if (-not $case.Error) { throw }
            $failed = $true
        }
        Assert-True ($failed -eq [bool]$case.Error) "$($case.Name): unexpected error outcome"
        $changes = @($global:C100CatalogTest.Calls | Where-Object { $_ -eq 'set-acl' }).Count
        Assert-True ($changes -eq [int]$case.Changes) "$($case.Name): unexpected ACL mutation"
        if ($changes) {
            Assert-True (($global:C100CatalogTest.Calls -join ',') -eq 'backup,set-acl') 'Backup must precede the ACL write.'
            Assert-True ($global:C100CatalogTest.Backup.OriginalSddl -eq $original) 'Original permissions must be recoverable.'
            Assert-True ($global:C100CatalogTest.WrittenPath -eq $catalog) 'Only the selected catalog may change.'
            $grants = @($global:C100CatalogTest.WrittenAcl.GetAccessRules($true,$true,[Security.Principal.SecurityIdentifier]) | Where-Object { $_.IdentityReference.Value -eq $serviceSid })
            Assert-True ($grants.Count -eq 1) 'Expected one service-specific grant.'
            Assert-True (($grants[0].FileSystemRights -band [Security.AccessControl.FileSystemRights]::Write) -eq 0) 'The service must not receive Write permission.'
        } else {
            Assert-True ($global:C100CatalogTest.Calls.Count -eq 0) 'Inspection/rejection must not create backups or change ACLs.'
        }
        Write-Host "PASS: $($case.Name)"
    }
} finally { Remove-Variable -Name C100CatalogTest -Scope Global -ErrorAction SilentlyContinue }
