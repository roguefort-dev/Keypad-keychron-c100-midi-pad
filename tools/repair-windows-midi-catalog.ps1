param(
    [Parameter(Mandatory=$true)][string]$CatalogPath,
    [switch]$Apply
)
$ErrorActionPreference = 'Stop'

# Run from an administrator PowerShell. Diagnosis is read-only unless -Apply
# is specified; this helper never starts elevation or restarts services.
$root = Join-Path $env:SystemRoot 'System32\CatRoot\{F750E6C3-38EE-11D1-85E5-00C04FC295EE}'
$path = [IO.Path]::GetFullPath($CatalogPath)
if (-not [string]::Equals([IO.Path]::GetDirectoryName($path), $root, [StringComparison]::OrdinalIgnoreCase) -or
    [IO.Path]::GetFileName($path) -notmatch '^Microsoft-Windows-Client-Desktop-Required-Package~31bf3856ad364e35~(amd64|arm64)~~[0-9.]+\.cat$') {
    throw 'Expected one Client-Desktop-Required-Package catalog directly in Windows CatRoot.'
}

$signature = Get-AuthenticodeSignature -LiteralPath $path
if ($signature.Status -ne 'Valid' -or
    $signature.SignerCertificate.Subject -notmatch 'CN=Microsoft Windows, O=Microsoft Corporation') {
    throw 'The catalog must have a valid Microsoft Windows signature.'
}
$hash = (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash
$acl = Get-Acl -LiteralPath $path
$service = [Security.Principal.NTAccount]::new('NT SERVICE\MidiSrv')
$serviceSid = $service.Translate([Security.Principal.SecurityIdentifier]).Value
$read = [Security.AccessControl.FileSystemRights]::Read
$rules = @($acl.GetAccessRules($true, $true, [Security.Principal.SecurityIdentifier]))
$hasRead = @($rules | Where-Object {
    $_.IdentityReference.Value -eq $serviceSid -and $_.AccessControlType -eq 'Allow' -and
    ($_.FileSystemRights -band $read) -eq $read
}).Count -gt 0

if ($Apply -and -not $hasRead) {
    # Recognize the observed fault exactly. Complex ACLs need separate diagnosis.
    $unexpected = @($rules | Where-Object {
        $_.IdentityReference.Value -notin @('S-1-5-18', 'S-1-5-32-544') -or
        $_.AccessControlType -ne 'Allow'
    })
    if (-not $acl.AreAccessRulesProtected -or $rules.Count -ne 2 -or $unexpected.Count -ne 0) {
        throw 'ACL does not match the observed SYSTEM/Administrators-only fault; nothing changed.'
    }
    $backupDirectory = Join-Path (Split-Path -Parent $PSScriptRoot) '.cache\windows-midi-catalog'
    New-Item -ItemType Directory -Path $backupDirectory -Force | Out-Null
    $backupFile = Join-Path $backupDirectory ("$hash-$([guid]::NewGuid()).json")
    [PSCustomObject]@{
        CatalogPath = $path
        SHA256 = $hash
        OriginalSddl = $acl.Sddl
        AddedServiceSid = $serviceSid
    } | ConvertTo-Json | Set-Content -LiteralPath $backupFile -Encoding UTF8
    $acl.AddAccessRule([Security.AccessControl.FileSystemAccessRule]::new(
        $service, $read, [Security.AccessControl.AccessControlType]::Allow))
    Set-Acl -LiteralPath $path -AclObject $acl
    if ((Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash -ne $hash) {
        throw 'Catalog contents changed unexpectedly; inspect before proceeding.'
    }
    $hasRead = $true
    Write-Host "Original permissions saved to $backupFile"
}

[PSCustomObject]@{
    CatalogPath = $path
    Signature = $signature.Status.ToString()
    SHA256 = $hash
    MidiServiceReadPermission = $hasRead
    Applied = [bool]($Apply -and $backupFile)
} | ConvertTo-Json
