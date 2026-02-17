import { renderInputControls, showInputSection, toggleInputState } from '../scripts/input-controls.js';
export { showInputSection, toggleInputState };

export function init()
{
	document.getElementById("SA_XX_VBAP-status").innerText = "initialized";
	document.getElementById("SA_XX_VBAP-status").style.backgroundColor = "";	
	document.getElementById("SA_XX_VBAP-Control_Version").innerText = "---";
	document.getElementById("SA_XX_VBAP-Control_SampleRate").innerText = "---";
    
    // Inject input controls
    renderInputControls('input-buttons-container', 'input-group-container', 'SA_XX_VBAP');
}

document.getElementById('SA_XX_VBAP-Total_VU').addEventListener('updated', (e) => 
{
	const vu = document.getElementById('SA_XX_VBAP-Total_VU');
	document.getElementById('SA_XX_VBAP-Total_VU-bar').style.height = vu.innerText + '%';
	document.getElementById('SA_XX_VBAP-Total_VU-number').innerText = Math.round(vu.innerText) + ' dB';
});			

document.getElementById('SA_XX_VBAP-Total_Volume').addEventListener('updated', (e) => 
{
	const vol = document.getElementById('SA_XX_VBAP-Total_Volume');
	document.getElementById('volume-slider').value = Math.round(vol.innerText);
	document.getElementById('volume-number').innerText = Math.round(vol.innerText) + " dB";
});	

export function connect()
{
	state = document.getElementById("SA_XX_VBAP-status");
	state.innerText = "connecting...";
	state.style.backgroundColor = "blue";
	sendResponse('/app/SA_XX_VBAP/osc/Control/Response', 9340);
	sendNoArgs('/app/SA_XX_VBAP/osc/Control/Version');
	secWaited = 0;
	setTimeout(() => { checkConnection(); }, 300);
}

export function checkConnection()
{
	const timeout = 1000; // timeout in ms
	secWaited += 100;
	state = document.getElementById("SA_XX_VBAP-Control_Version");
	console.log("waiting for " + secWaited/1000 + "s, version=" + state.innerText);
	switch (state.innerText)
	{
		case "---":
			if (secWaited <= timeout)
			{		// try again
				sendResponse('/app/SA_XX_VBAP/osc/Control/Response');
				sendNoArgs('/app/SA_XX_VBAP/osc/Control/Version');		
				setTimeout(() => { checkConnection(); }, 100);
			}
			break;
		default: // done!
			state = document.getElementById("SA_XX_VBAP-status");
			state.innerText = "connected";
			state.style.backgroundColor = "";
			sendNoArgs('/app/SA_XX_VBAP/osc/Control/SampleRate');
			sendNoArgs('/app/SA_XX_VBAP/osc/Total/Volume');
			break;
	}
	if (secWaited > timeout)
	{ 	// time out -> clean up
		state = document.getElementById("SA_XX_VBAP-status");
		state.innerText = "time out while connecting";
		state.style.backgroundColor = "red";
	}
}		
