// web_page.h
#ifndef WEB_PAGE_H
#define WEB_PAGE_H

const char webpage_html[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>TurtleScreen Setup</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;background:#1a1a2e;color:#eee;min-height:100vh;display:flex;align-items:center;justify-content:center;padding:20px}
.card{background:#16213e;border-radius:12px;padding:32px;width:100%;max-width:420px;box-shadow:0 8px 32px rgba(0,0,0,0.3)}
h1{font-size:1.4em;margin-bottom:8px;color:#4fc3f7}
.subtitle{font-size:0.85em;color:#888;margin-bottom:24px}
label{display:block;font-size:0.85em;color:#aaa;margin-bottom:4px;margin-top:16px}
input[type="text"],input[type="password"]{width:100%;padding:10px 12px;border:1px solid #333;border-radius:6px;background:#0f3460;color:#eee;font-size:1em;outline:none;transition:border-color 0.2s}
input[type="text"]:focus,input[type="password"]:focus{border-color:#4fc3f7}
.btn{display:block;width:100%;padding:12px;margin-top:24px;border:none;border-radius:6px;background:#4fc3f7;color:#1a1a2e;font-size:1em;font-weight:600;cursor:pointer;transition:background 0.2s}
.btn:hover{background:#81d4fa}
.btn:disabled{background:#555;cursor:not-allowed}
.msg{margin-top:16px;padding:12px;border-radius:6px;font-size:0.9em;display:none}
.msg.success{display:block;background:#1b5e20;color:#a5d6a7}
.msg.error{display:block;background:#b71c1c;color:#ef9a9a}
.nav{margin-top:20px;text-align:center}
.nav a{color:#4fc3f7;text-decoration:none;font-size:0.85em}
.nav a:hover{text-decoration:underline}
</style>
</head>
<body>
<div class="card">
<h1>TurtleScreen Setup</h1>
<p class="subtitle">Configure Wi-Fi and printer connection</p>
<form id="configForm">
<label for="ssid">Wi-Fi SSID</label>
<input type="text" id="ssid" name="ssid" placeholder="Your network name" required>
<label for="password">Wi-Fi Password</label>
<input type="password" id="password" name="password" placeholder="Network password" required>
<label for="host">Printer IP / Hostname</label>
<input type="text" id="host" name="host" placeholder="e.g. 192.168.1.100" required>
<button type="submit" class="btn" id="submitBtn">Save & Connect</button>
</form>
<div id="msg" class="msg"></div>
<div class="nav"><a href="/update">Firmware Update &rarr;</a></div>
</div>
<script>
document.getElementById('configForm').addEventListener('submit', function(e) {
    e.preventDefault();
    var btn = document.getElementById('submitBtn');
    var msg = document.getElementById('msg');
    btn.disabled = true;
    btn.textContent = 'Saving...';
    msg.className = 'msg';
    msg.style.display = 'none';
    var formData = new FormData(this);
    fetch('/setConfig', {method:'POST', body: formData})
    .then(function(r) {
        if (!r.ok) throw new Error('HTTP ' + r.status);
        return r.text();
    })
    .then(function(t) {
        msg.className = 'msg success';
        msg.textContent = t;
        msg.style.display = 'block';
        btn.textContent = 'Saved!';
    })
    .catch(function(err) {
        msg.className = 'msg error';
        msg.textContent = 'Error: ' + err.message;
        msg.style.display = 'block';
        btn.disabled = false;
        btn.textContent = 'Save & Connect';
    });
});
</script>
</body>
</html>
)rawliteral";

const char upload_html[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>TurtleScreen Firmware Update</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;background:#1a1a2e;color:#eee;min-height:100vh;display:flex;align-items:center;justify-content:center;padding:20px}
.card{background:#16213e;border-radius:12px;padding:32px;width:100%;max-width:420px;box-shadow:0 8px 32px rgba(0,0,0,0.3)}
h1{font-size:1.4em;margin-bottom:8px;color:#4fc3f7}
.subtitle{font-size:0.85em;color:#888;margin-bottom:24px}
.file-input{position:relative;width:100%;margin-top:16px}
.file-input input[type="file"]{position:absolute;opacity:0;width:100%;height:100%;cursor:pointer;top:0;left:0}
.file-label{display:flex;align-items:center;justify-content:center;padding:24px;border:2px dashed #333;border-radius:8px;color:#888;font-size:0.9em;transition:border-color 0.2s,color 0.2s;text-align:center}
.file-label.has-file{border-color:#4fc3f7;color:#4fc3f7}
.file-label:hover{border-color:#4fc3f7}
.btn{display:block;width:100%;padding:12px;margin-top:20px;border:none;border-radius:6px;background:#4fc3f7;color:#1a1a2e;font-size:1em;font-weight:600;cursor:pointer;transition:background 0.2s}
.btn:hover{background:#81d4fa}
.btn:disabled{background:#555;cursor:not-allowed}
.progress-wrap{margin-top:20px;display:none}
.progress-bar{width:100%;height:8px;background:#0f3460;border-radius:4px;overflow:hidden}
.progress-fill{height:100%;width:0%;background:linear-gradient(90deg,#4fc3f7,#81d4fa);border-radius:4px;transition:width 0.3s}
.progress-text{font-size:0.8em;color:#aaa;margin-top:6px;text-align:center}
.msg{margin-top:16px;padding:12px;border-radius:6px;font-size:0.9em;display:none}
.msg.success{display:block;background:#1b5e20;color:#a5d6a7}
.msg.error{display:block;background:#b71c1c;color:#ef9a9a}
.nav{margin-top:20px;text-align:center}
.nav a{color:#4fc3f7;text-decoration:none;font-size:0.85em}
.nav a:hover{text-decoration:underline}
.info{margin-top:16px;padding:10px;background:#0f3460;border-radius:6px;font-size:0.8em;color:#888}
</style>
</head>
<body>
<div class="card">
<h1>Firmware Update</h1>
<p class="subtitle">Upload a new firmware binary (.bin)</p>
<form id="uploadForm">
<div class="file-input">
<div class="file-label" id="fileLabel">Drop firmware.bin here or click to browse</div>
<input type="file" id="fileInput" name="firmware" accept=".bin">
</div>
<div id="fileInfo" class="info" style="display:none"></div>
<button type="submit" class="btn" id="uploadBtn" disabled>Upload Firmware</button>
</form>
<div class="progress-wrap" id="progressWrap">
<div class="progress-bar"><div class="progress-fill" id="progressFill"></div></div>
<div class="progress-text" id="progressText">0%</div>
</div>
<div id="msg" class="msg"></div>
<div class="nav"><a href="/">&larr; Back to Setup</a></div>
</div>
<script>
var fileInput = document.getElementById('fileInput');
var fileLabel = document.getElementById('fileLabel');
var fileInfo = document.getElementById('fileInfo');
var uploadBtn = document.getElementById('uploadBtn');
var progressWrap = document.getElementById('progressWrap');
var progressFill = document.getElementById('progressFill');
var progressText = document.getElementById('progressText');
var msg = document.getElementById('msg');

fileInput.addEventListener('change', function() {
    if (this.files.length > 0) {
        var file = this.files[0];
        fileLabel.textContent = file.name;
        fileLabel.classList.add('has-file');
        var sizeKB = (file.size / 1024).toFixed(1);
        fileInfo.textContent = 'Size: ' + sizeKB + ' KB';
        fileInfo.style.display = 'block';
        uploadBtn.disabled = false;
        if (!file.name.endsWith('.bin')) {
            msg.className = 'msg error';
            msg.textContent = 'Warning: File does not have .bin extension';
            msg.style.display = 'block';
            uploadBtn.disabled = true;
        } else {
            msg.style.display = 'none';
        }
    }
});

document.getElementById('uploadForm').addEventListener('submit', function(e) {
    e.preventDefault();
    var file = fileInput.files[0];
    if (!file) return;

    uploadBtn.disabled = true;
    uploadBtn.textContent = 'Uploading...';
    msg.style.display = 'none';
    progressWrap.style.display = 'block';
    progressFill.style.width = '0%';
    progressText.textContent = '0%';

    var xhr = new XMLHttpRequest();
    xhr.open('POST', '/update', true);

    xhr.upload.addEventListener('progress', function(evt) {
        if (evt.lengthComputable) {
            var pct = Math.round((evt.loaded / evt.total) * 100);
            progressFill.style.width = pct + '%';
            progressText.textContent = pct + '% (' + (evt.loaded / 1024).toFixed(0) + ' / ' + (evt.total / 1024).toFixed(0) + ' KB)';
        }
    });

    xhr.addEventListener('load', function() {
        if (xhr.status === 200) {
            progressFill.style.width = '100%';
            progressText.textContent = '100% - Complete!';
            msg.className = 'msg success';
            msg.textContent = 'Firmware uploaded successfully! Device is rebooting...';
            msg.style.display = 'block';
            uploadBtn.textContent = 'Rebooting...';
            setTimeout(function() {
                msg.textContent = 'Firmware uploaded successfully! Device is rebooting... Refreshing in 10s.';
                setTimeout(function() { location.reload(); }, 10000);
            }, 2000);
        } else {
            msg.className = 'msg error';
            msg.textContent = 'Upload failed: Server returned HTTP ' + xhr.status + '. ' + xhr.responseText;
            msg.style.display = 'block';
            uploadBtn.disabled = false;
            uploadBtn.textContent = 'Upload Firmware';
        }
    });

    xhr.addEventListener('error', function() {
        msg.className = 'msg error';
        msg.textContent = 'Upload failed: Connection error. Check that the device is reachable.';
        msg.style.display = 'block';
        uploadBtn.disabled = false;
        uploadBtn.textContent = 'Upload Firmware';
    });

    xhr.addEventListener('timeout', function() {
        msg.className = 'msg error';
        msg.textContent = 'Upload failed: Request timed out. The file may be too large or connection too slow.';
        msg.style.display = 'block';
        uploadBtn.disabled = false;
        uploadBtn.textContent = 'Upload Firmware';
    });

    xhr.timeout = 120000;
    var formData = new FormData();
    formData.append('firmware', file);
    xhr.send(formData);
});
</script>
</body>
</html>
)rawliteral";

#endif // WEB_PAGE_H
