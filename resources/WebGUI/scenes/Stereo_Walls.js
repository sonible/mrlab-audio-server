import { renderInputControls, showInputSection, toggleInputState } from '../scripts/input-controls.js';
export { showInputSection, toggleInputState };

export function init()
{
	document.getElementById("Stereo_Walls-status").innerText = "initialized";
	document.getElementById("Stereo_Walls-status").style.backgroundColor = "";	
	document.getElementById("Stereo_Walls-Control_Version").innerText = "---";
    // Inject input controls
    renderInputControls('input-buttons-container', 'input-group-container', 'Stereo_Walls');
}

document.getElementById('Stereo_Walls-Total_VU').addEventListener('updated', (e) => 
{
	const vu = document.getElementById('Stereo_Walls-Total_VU');
	document.getElementById('Stereo_Walls-Total_VU-bar').style.height = vu.innerText + '%';
	document.getElementById('Stereo_Walls-Total_VU-number').innerText = Math.round(vu.innerText) + ' dB';
});			

document.getElementById('Stereo_Walls-Total_Volume').addEventListener('updated', (e) => 
{
	const vol = document.getElementById('Stereo_Walls-Total_Volume');
	document.getElementById('volume-slider').value = Math.round(vol.innerText);
	document.getElementById('volume-number').innerText = Math.round(vol.innerText) + " dB";
});	

document.getElementById('Stereo_Walls-CAVE_Volume').addEventListener('updated', (e) => 
{
	const vol = document.getElementById('Stereo_Walls-CAVE_Volume');
	document.getElementById('volume-cave-slider').value = Math.round(vol.innerText);
	document.getElementById('volume-cave-number').innerText = Math.round(vol.innerText) + " dB";
});	

document.getElementById('Stereo_Walls-CAVEDoor_Volume').addEventListener('updated', (e) => 
{
	const vol = document.getElementById('Stereo_Walls-CAVEDoor_Volume');
	document.getElementById('volume-cavedoor-slider').value = Math.round(vol.innerText);
	document.getElementById('volume-cavedoor-number').innerText = Math.round(vol.innerText) + " dB";
});	

document.getElementById('Stereo_Walls-SA_Volume').addEventListener('updated', (e) => 
{
	const vol = document.getElementById('Stereo_Walls-SA_Volume');
	document.getElementById('volume-sa-slider').value = Math.round(vol.innerText);
	document.getElementById('volume-sa-number').innerText = Math.round(vol.innerText) + " dB";
});	

document.getElementById('Stereo_Walls-CurvedDoor_Volume').addEventListener('updated', (e) => 
{
	const vol = document.getElementById('Stereo_Walls-CurvedDoor_Volume');
	document.getElementById('volume-curveddoor-slider').value = Math.round(vol.innerText);
	document.getElementById('volume-curveddoor-number').innerText = Math.round(vol.innerText) + " dB";
});	

document.getElementById('Stereo_Walls-Curved_Volume').addEventListener('updated', (e) => 
{
	const vol = document.getElementById('Stereo_Walls-Curved_Volume');
	document.getElementById('volume-curved-slider').value = Math.round(vol.innerText);
	document.getElementById('volume-curved-number').innerText = Math.round(vol.innerText) + " dB";
});	

export function connect()
{
	state = document.getElementById("Stereo_Walls-status");
	state.innerText = "connecting...";
	state.style.backgroundColor = "blue";
	sendResponse('/app/Stereo_Walls/osc/Control/Response', 9336);
	sendNoArgs('/app/Stereo_Walls/osc/Control/Version');
	secWaited = 0;
	setTimeout(() => { checkConnection(); }, 300);
}

export function checkConnection()
{
	const timeout = 1000; // timeout in ms
	secWaited += 100;
	state = document.getElementById("Stereo_Walls-Control_Version");
	//console.log("waiting for " + secWaited/1000 + "s, version=" + state.innerText);
	switch (state.innerText)
	{
		case "---":
			if (secWaited <= timeout)
			{		// try again
				sendResponse('/app/Stereo_Walls/osc/Control/Response');
				sendNoArgs('/app/Stereo_Walls/osc/Control/Version');		
				setTimeout(() => { checkConnection(); }, 100);
			}
			break;
		default: // done!
      console.log("Stereo_Walls: connected");
			state = document.getElementById("Stereo_Walls-status");
			state.innerText = "connected";
			state.style.backgroundColor = "";
			sendNoArgs('/app/Stereo_Walls/osc/Total/Volume');
			break;
	}
	if (secWaited > timeout)
	{ 	// time out -> clean up
		state = document.getElementById("Stereo_Walls-status");
		state.innerText = "time out while connecting";
		state.style.backgroundColor = "red";
	}
}		
