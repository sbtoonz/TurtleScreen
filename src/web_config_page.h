#ifndef WEB_CONFIG_PAGE_H
#define WEB_CONFIG_PAGE_H

const char config_html[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>TurtleScreen AFC Config</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;background:#1a1a2e;color:#eee;min-height:100vh;padding:20px}
.container{max-width:700px;margin:0 auto}
.card{background:#16213e;border-radius:12px;padding:24px;margin-bottom:20px;box-shadow:0 8px 32px rgba(0,0,0,0.3)}
h1{font-size:1.4em;color:#4fc3f7;margin-bottom:4px}
h2{font-size:1.1em;color:#81d4fa;margin-bottom:12px}
.subtitle{font-size:0.85em;color:#888;margin-bottom:16px}
label{display:block;font-size:0.8em;color:#aaa;margin-bottom:2px;margin-top:10px}
input[type="text"]{width:100%;padding:8px 10px;border:1px solid #333;border-radius:4px;background:#0f3460;color:#eee;font-size:0.9em;font-family:monospace;outline:none}
input[type="text"]:focus{border-color:#4fc3f7}
.row{display:grid;grid-template-columns:1fr 1fr;gap:12px}
.btn{display:inline-block;padding:10px 20px;margin-top:16px;margin-right:8px;border:none;border-radius:6px;font-size:0.9em;font-weight:600;cursor:pointer;transition:background 0.2s}
.btn-primary{background:#4fc3f7;color:#1a1a2e}
.btn-primary:hover{background:#81d4fa}
.btn-secondary{background:#333;color:#eee}
.btn-secondary:hover{background:#444}
.btn-danger{background:#c62828;color:#fff}
.btn-danger:hover{background:#e53935}
.btn:disabled{opacity:0.5;cursor:not-allowed}
.msg{margin-top:12px;padding:10px;border-radius:6px;font-size:0.85em;display:none}
.msg.success{display:block;background:#1b5e20;color:#a5d6a7}
.msg.error{display:block;background:#b71c1c;color:#ef9a9a}
.msg.info{display:block;background:#0d47a1;color:#90caf9}
textarea{width:100%;min-height:200px;padding:10px;border:1px solid #333;border-radius:6px;background:#0f3460;color:#eee;font-family:monospace;font-size:0.8em;resize:vertical;outline:none}
textarea:focus{border-color:#4fc3f7}
.nav{text-align:center;margin-top:20px}
.nav a{color:#4fc3f7;text-decoration:none;font-size:0.85em;margin:0 12px}
.nav a:hover{text-decoration:underline}
.live-data{background:#0a1628;border:1px solid #333;border-radius:6px;padding:12px;font-family:monospace;font-size:0.75em;max-height:300px;overflow-y:auto;white-space:pre-wrap;word-break:break-all;color:#76ff03}
.section-sep{border:none;border-top:1px solid #333;margin:16px 0}
</style>
</head>
<body>
<div class="container">

<div class="card">
<h1>AFC API Configuration</h1>
<p class="subtitle">Edit command names and field mappings. Changes take effect immediately — no reflash needed.</p>
</div>

<div class="card">
<h2>GCode Commands</h2>
<p class="subtitle">These are sent to Klipper when buttons are pressed on the screen.</p>
<div class="row">
<div><label>Tool Change</label><input type="text" id="cmd_tool_change"></div>
<div><label>Tool Unload</label><input type="text" id="cmd_tool_unload"></div>
</div>
<div class="row">
<div><label>Lane Unload</label><input type="text" id="cmd_lane_unload"></div>
<div><label>Prep</label><input type="text" id="cmd_prep"></div>
</div>
<div class="row">
<div><label>Cut</label><input type="text" id="cmd_cut"></div>
<div><label>Brush/Wipe</label><input type="text" id="cmd_brush"></div>
</div>
<div class="row">
<div><label>Kick</label><input type="text" id="cmd_kick"></div>
<div><label>Park</label><input type="text" id="cmd_park"></div>
</div>
<div class="row">
<div><label>Poop</label><input type="text" id="cmd_poop"></div>
<div></div>
</div>
</div>

<div class="card">
<h2>API Field Mappings</h2>
<p class="subtitle">JSON keys used when parsing the AFC status response.</p>
<div class="row">
<div><label>Status Key</label><input type="text" id="fld_status_key"></div>
<div><label>Endpoint Path</label><input type="text" id="fld_endpoint"></div>
</div>
<hr class="section-sep">
<p class="subtitle">Lane fields</p>
<div class="row">
<div><label>Load</label><input type="text" id="fld_lane_load"></div>
<div><label>Prep</label><input type="text" id="fld_lane_prep"></div>
</div>
<div class="row">
<div><label>Tool Loaded</label><input type="text" id="fld_lane_tool_loaded"></div>
<div><label>Loaded to Hub</label><input type="text" id="fld_lane_hub_loaded"></div>
</div>
<div class="row">
<div><label>Material</label><input type="text" id="fld_lane_material"></div>
<div><label>Color</label><input type="text" id="fld_lane_color"></div>
</div>
<div class="row">
<div><label>Weight</label><input type="text" id="fld_lane_weight"></div>
<div><label>Spool ID</label><input type="text" id="fld_lane_spool_id"></div>
</div>
<div class="row">
<div><label>Lane Index</label><input type="text" id="fld_lane_index"></div>
<div><label>Lane Map (tool#)</label><input type="text" id="fld_lane_map"></div>
</div>
<hr class="section-sep">
<p class="subtitle">System fields</p>
<div class="row">
<div><label>Current Load</label><input type="text" id="fld_sys_current_load"></div>
<div><label>Extruder Tool Status</label><input type="text" id="fld_ext_tool_status"></div>
</div>
<div class="row">
<div><label>Num Units</label><input type="text" id="fld_sys_num_units"></div>
<div><label>Num Lanes</label><input type="text" id="fld_sys_num_lanes"></div>
</div>
<div class="row">
<div><label>Hub Loaded (unit)</label><input type="text" id="fld_unit_hub_loaded"></div>
<div><label>Unit Type</label><input type="text" id="fld_unit_type"></div>
</div>
</div>

<div class="card">
<h2>Save / Load</h2>
<button class="btn btn-primary" onclick="saveConfig()">Save to Device</button>
<button class="btn btn-secondary" onclick="loadConfig()">Reload from Device</button>
<button class="btn btn-danger" onclick="resetDefaults()">Reset to Defaults</button>
<div id="saveMsg" class="msg"></div>
</div>

<div class="card">
<h2>Remote Config URL</h2>
<p class="subtitle">Paste a URL to a JSON config file. The device will download and apply it.</p>
<label>Config URL</label>
<input type="text" id="remoteUrl" placeholder="https://raw.githubusercontent.com/...">
<button class="btn btn-primary" onclick="fetchRemote()">Fetch & Apply</button>
<div id="remoteMsg" class="msg"></div>
</div>

<div class="card">
<h2>Live API Response</h2>
<p class="subtitle">Raw response from the AFC status endpoint. Useful for verifying field names.</p>
<button class="btn btn-secondary" onclick="fetchLive()">Fetch Live Data</button>
<div id="liveMsg" class="msg"></div>
<div id="liveData" class="live-data" style="margin-top:12px;display:none"></div>
</div>

<div class="card">
<h2>Raw JSON Config</h2>
<p class="subtitle">Advanced: edit the full config JSON directly or paste one in.</p>
<textarea id="rawJson"></textarea>
<button class="btn btn-secondary" onclick="applyRaw()">Apply JSON</button>
<div id="rawMsg" class="msg"></div>
</div>

<div class="nav">
<a href="/">&larr; Setup</a>
<a href="/update">Firmware Update</a>
</div>

</div>
<script>
function showMsg(el, cls, txt) {
    el.className = 'msg ' + cls;
    el.textContent = txt;
    el.style.display = 'block';
}

function populateForm(cfg) {
    document.getElementById('fld_endpoint').value = cfg.endpoint || '';
    document.getElementById('remoteUrl').value = cfg.config_url || '';
    var c = cfg.commands || {};
    document.getElementById('cmd_tool_change').value = c.tool_change || '';
    document.getElementById('cmd_tool_unload').value = c.tool_unload || '';
    document.getElementById('cmd_lane_unload').value = c.lane_unload || '';
    document.getElementById('cmd_prep').value = c.prep || '';
    document.getElementById('cmd_cut').value = c.cut || '';
    document.getElementById('cmd_brush').value = c.brush || '';
    document.getElementById('cmd_kick').value = c.kick || '';
    document.getElementById('cmd_park').value = c.park || '';
    document.getElementById('cmd_poop').value = c.poop || '';
    var f = cfg.fields || {};
    document.getElementById('fld_status_key').value = f.status_key || '';
    document.getElementById('fld_lane_load').value = f.lane_load || '';
    document.getElementById('fld_lane_prep').value = f.lane_prep || '';
    document.getElementById('fld_lane_tool_loaded').value = f.lane_tool_loaded || '';
    document.getElementById('fld_lane_hub_loaded').value = f.lane_hub_loaded || '';
    document.getElementById('fld_lane_material').value = f.lane_material || '';
    document.getElementById('fld_lane_color').value = f.lane_color || '';
    document.getElementById('fld_lane_weight').value = f.lane_weight || '';
    document.getElementById('fld_lane_spool_id').value = f.lane_spool_id || '';
    document.getElementById('fld_lane_index').value = f.lane_index || '';
    document.getElementById('fld_lane_map').value = f.lane_map || '';
    document.getElementById('fld_sys_current_load').value = f.sys_current_load || '';
    document.getElementById('fld_sys_num_units').value = f.sys_num_units || '';
    document.getElementById('fld_sys_num_lanes').value = f.sys_num_lanes || '';
    document.getElementById('fld_ext_tool_status').value = f.ext_tool_status || '';
    document.getElementById('fld_unit_hub_loaded').value = f.unit_hub_loaded || '';
    document.getElementById('fld_unit_type').value = f.unit_type || '';
    document.getElementById('rawJson').value = JSON.stringify(cfg, null, 2);
}

function gatherForm() {
    return {
        endpoint: document.getElementById('fld_endpoint').value,
        config_url: document.getElementById('remoteUrl').value,
        commands: {
            tool_change: document.getElementById('cmd_tool_change').value,
            tool_unload: document.getElementById('cmd_tool_unload').value,
            lane_unload: document.getElementById('cmd_lane_unload').value,
            prep: document.getElementById('cmd_prep').value,
            cut: document.getElementById('cmd_cut').value,
            brush: document.getElementById('cmd_brush').value,
            kick: document.getElementById('cmd_kick').value,
            park: document.getElementById('cmd_park').value,
            poop: document.getElementById('cmd_poop').value
        },
        fields: {
            status_key: document.getElementById('fld_status_key').value,
            lane_load: document.getElementById('fld_lane_load').value,
            lane_prep: document.getElementById('fld_lane_prep').value,
            lane_tool_loaded: document.getElementById('fld_lane_tool_loaded').value,
            lane_hub_loaded: document.getElementById('fld_lane_hub_loaded').value,
            lane_material: document.getElementById('fld_lane_material').value,
            lane_color: document.getElementById('fld_lane_color').value,
            lane_weight: document.getElementById('fld_lane_weight').value,
            lane_spool_id: document.getElementById('fld_lane_spool_id').value,
            lane_index: document.getElementById('fld_lane_index').value,
            lane_map: document.getElementById('fld_lane_map').value,
            sys_current_load: document.getElementById('fld_sys_current_load').value,
            sys_num_units: document.getElementById('fld_sys_num_units').value,
            sys_num_lanes: document.getElementById('fld_sys_num_lanes').value,
            ext_tool_status: document.getElementById('fld_ext_tool_status').value,
            unit_hub_loaded: document.getElementById('fld_unit_hub_loaded').value,
            unit_type: document.getElementById('fld_unit_type').value
        }
    };
}

function loadConfig() {
    fetch('/api/config').then(function(r){ return r.json(); }).then(function(cfg){
        populateForm(cfg);
        showMsg(document.getElementById('saveMsg'), 'success', 'Config loaded from device');
    }).catch(function(e){
        showMsg(document.getElementById('saveMsg'), 'error', 'Failed to load: ' + e.message);
    });
}

function saveConfig() {
    var cfg = gatherForm();
    document.getElementById('rawJson').value = JSON.stringify(cfg, null, 2);
    fetch('/api/config', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify(cfg)
    }).then(function(r){
        if (!r.ok) throw new Error('HTTP ' + r.status);
        return r.text();
    }).then(function(t){
        showMsg(document.getElementById('saveMsg'), 'success', t);
    }).catch(function(e){
        showMsg(document.getElementById('saveMsg'), 'error', 'Save failed: ' + e.message);
    });
}

function resetDefaults() {
    if (!confirm('Reset all config to factory defaults?')) return;
    fetch('/api/config/reset', {method:'POST'}).then(function(r){
        if (!r.ok) throw new Error('HTTP ' + r.status);
        return r.text();
    }).then(function(){
        loadConfig();
        showMsg(document.getElementById('saveMsg'), 'info', 'Reset to defaults');
    }).catch(function(e){
        showMsg(document.getElementById('saveMsg'), 'error', 'Reset failed: ' + e.message);
    });
}

function fetchRemote() {
    var url = document.getElementById('remoteUrl').value.trim();
    if (!url) { showMsg(document.getElementById('remoteMsg'), 'error', 'Enter a URL'); return; }
    showMsg(document.getElementById('remoteMsg'), 'info', 'Fetching...');
    fetch('/api/config/fetch?url=' + encodeURIComponent(url), {method:'POST'}).then(function(r){
        if (!r.ok) throw new Error('HTTP ' + r.status);
        return r.text();
    }).then(function(t){
        showMsg(document.getElementById('remoteMsg'), 'success', t);
        loadConfig();
    }).catch(function(e){
        showMsg(document.getElementById('remoteMsg'), 'error', 'Fetch failed: ' + e.message);
    });
}

function fetchLive() {
    showMsg(document.getElementById('liveMsg'), 'info', 'Fetching from printer...');
    fetch('/api/live').then(function(r){
        if (!r.ok) throw new Error('HTTP ' + r.status);
        return r.text();
    }).then(function(t){
        document.getElementById('liveMsg').style.display = 'none';
        var el = document.getElementById('liveData');
        try { el.textContent = JSON.stringify(JSON.parse(t), null, 2); }
        catch(e) { el.textContent = t; }
        el.style.display = 'block';
    }).catch(function(e){
        showMsg(document.getElementById('liveMsg'), 'error', 'Failed: ' + e.message);
    });
}

function applyRaw() {
    var raw = document.getElementById('rawJson').value.trim();
    try {
        var cfg = JSON.parse(raw);
        populateForm(cfg);
        showMsg(document.getElementById('rawMsg'), 'success', 'Applied to form. Click "Save to Device" to persist.');
    } catch(e) {
        showMsg(document.getElementById('rawMsg'), 'error', 'Invalid JSON: ' + e.message);
    }
}

loadConfig();
</script>
</body>
</html>
)rawliteral";

#endif // WEB_CONFIG_PAGE_H
