param(
    [string]$Python = 'python',
    [switch]$Elevated
)
$ErrorActionPreference = 'Stop'
$project = Split-Path -Parent $PSScriptRoot
$cache = Join-Path $project '.cache\c100-windows'
$pythonPath = (Get-Command $Python).Source

# Downloads are verified before requesting Windows elevation.
& $pythonPath (Join-Path $PSScriptRoot 'flash_c100.py') --setup
if ($LASTEXITCODE -ne 0) { throw 'Windows tool setup failed.' }

$identity = [Security.Principal.WindowsIdentity]::GetCurrent()
$principal = [Security.Principal.WindowsPrincipal]::new($identity)
if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    if ($Elevated) { throw 'Windows administrator rights are required for initial driver installation.' }
    Write-Host 'Windows needs a one-time UAC approval to install the C100 DFU driver and refresh its USB binding.'
    $arguments = @('-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', "`"$PSCommandPath`"",
                   '-Python', "`"$pythonPath`"", '-Elevated')
    $process = Start-Process -FilePath (Get-Process -Id $PID).Path -ArgumentList $arguments `
        -Verb RunAs -WindowStyle Hidden -PassThru -Wait
    Get-Content (Join-Path $cache 'setup.log') -ErrorAction SilentlyContinue
    exit $process.ExitCode
}

$exitCode = 1
Start-Transcript -Path (Join-Path $cache 'setup.log') -Force | Out-Null
try {
    # --all stages the driver even when the keyboard is still in normal mode.
    & (Join-Path $cache 'qmk_driver_installer.exe') --all --force (Join-Path $PSScriptRoot 'c100-dfu-driver.txt')
    if ($LASTEXITCODE -ne 0) { throw "C100 DFU driver installation failed ($LASTEXITCODE)." }

    & $pythonPath (Join-Path $PSScriptRoot 'flash_c100.py') --check
    if ($LASTEXITCODE -ne 0) {
        # Discard only the C100 composite instance so Windows reads descriptors
        # again. Keep shared USB/HID/audio driver packages installed.
        $devices = @(Get-PnpDevice -PresentOnly | Where-Object {
            $_.InstanceId -match '^USB\\VID_3434&PID_042C\\'
        })
        if ($devices.Count -gt 1) { throw 'Multiple C100 devices connected; leave only one connected for setup.' }
        foreach ($device in $devices) {
            & pnputil /remove-device $device.InstanceId /subtree
            if ($LASTEXITCODE -ne 0) { throw 'Could not remove the stale C100 device instance.' }
        }
        & pnputil /scan-devices
        if ($LASTEXITCODE -ne 0) { throw 'USB device rescan failed.' }
        Start-Sleep -Seconds 3
        & $pythonPath (Join-Path $PSScriptRoot 'flash_c100.py') --check
        if ($LASTEXITCODE -ne 0) {
            throw 'No usable C100 MIDI or DFU interface. One initial manual DFU entry and flash may be needed; see docs/windows-flashing.md.'
        }
    }
    Write-Host 'C100 setup complete. Future flashes need no elevation or physical key presses.'
    $exitCode = 0
}
catch { Write-Host "Setup incomplete: $_" }
finally { Stop-Transcript | Out-Null }
exit $exitCode
