import { renderInputControls, showInputSection, toggleInputState } from '../scripts/input-controls.js';
export { showInputSection, toggleInputState };

export function init()
{
	document.getElementById("Curved_LED_Stereo-status").innerText = "initialized";
	document.getElementById("Curved_LED_Stereo-status").style.backgroundColor = "";	
	document.getElementById("Curved_LED_Stereo-Control_Version").innerText = "---";
	document.getElementById("Curved_LED_Stereo-Control_SampleRate").innerText = "---";
    // Inject input controls
    renderInputControls('input-buttons-container', 'input-group-container', 'Curved_LED_Stereo');
}

document.getElementById('Curved_LED_Stereo-Total_VU').addEventListener('updated', (e) => 
{
	const vu = document.getElementById('Curved_LED_Stereo-Total_VU');
	document.getElementById('Curved_LED_Stereo-Total_VU-bar').style.height = vu.innerText + '%';
	document.getElementById('Curved_LED_Stereo-Total_VU-number').innerText = Math.round(vu.innerText) + ' dB';
});			

document.getElementById('Curved_LED_Stereo-Total_Volume').addEventListener('updated', (e) => 
{
	const vol = document.getElementById('Curved_LED_Stereo-Total_Volume');
	document.getElementById('volume-slider').value = Math.round(vol.innerText);
	document.getElementById('volume-number').innerText = Math.round(vol.innerText) + " dB";
});	

export function connect()
{
	state = document.getElementById("Curved_LED_Stereo-status");
	state.innerText = "connecting...";
	state.style.backgroundColor = "blue";
	sendResponse('/app/Curved_LED_Stereo/osc/Control/Response', 9340);
	sendNoArgs('/app/Curved_LED_Stereo/osc/Control/Version');
	secWaited = 0;
	setTimeout(() => { checkConnection(); }, 300);
}

export function checkConnection()
{
	const timeout = 1000; // timeout in ms
	secWaited += 100;
	state = document.getElementById("Curved_LED_Stereo-Control_Version");
	console.log("waiting for " + secWaited/1000 + "s, version=" + state.innerText);
	switch (state.innerText)
	{
		case "---":
			if (secWaited <= timeout)
			{		// try again
				sendResponse('/app/Curved_LED_Stereo/osc/Control/Response');
				sendNoArgs('/app/Curved_LED_Stereo/osc/Control/Version');		
				setTimeout(() => { checkConnection(); }, 100);
			}
			break;
		default: // done!
			state = document.getElementById("Curved_LED_Stereo-status");
			state.innerText = "connected";
			state.style.backgroundColor = "";
			sendNoArgs('/app/Curved_LED_Stereo/osc/Control/SampleRate');
			sendNoArgs('/app/Curved_LED_Stereo/osc/Total/Volume');
			break;
	}
	if (secWaited > timeout)
	{ 	// time out -> clean up
		state = document.getElementById("Curved_LED_Stereo-status");
		state.innerText = "time out while connecting";
		state.style.backgroundColor = "red";
	}
}		
