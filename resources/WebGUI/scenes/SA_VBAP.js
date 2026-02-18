import { renderInputControls, showInputSection, toggleInputState } from '../scripts/input-controls.js';
export { showInputSection, toggleInputState };

export function init()
{
	document.getElementById("SA_VBAP-status").innerText = "initialized";
	document.getElementById("SA_VBAP-status").style.backgroundColor = "";	
	document.getElementById("SA_VBAP-Control_Version").innerText = "---";
	document.getElementById("SA_VBAP-Control_SampleRate").innerText = "---"; 
	document.getElementById("SA_VBAP-Control_Door").innerText = "---";
    // Inject input controls
    renderInputControls('input-buttons-container', 'input-group-container', 'SA_VBAP');
}

document.getElementById('SA_VBAP-Total_VU').addEventListener('updated', (e) => 
{
	const vu = document.getElementById('SA_VBAP-Total_VU');
	document.getElementById('SA_VBAP-Total_VU-bar').style.height = vu.innerText + '%';
	document.getElementById('SA_VBAP-Total_VU-number').innerText = Math.round(vu.innerText) + ' dB';
});			

document.getElementById('SA_VBAP-Total_Volume').addEventListener('updated', (e) => 
{
	const vol = document.getElementById('SA_VBAP-Total_Volume');
	document.getElementById('volume-slider').value = Math.round(vol.innerText);
	document.getElementById('volume-number').innerText = Math.round(vol.innerText) + " dB";
});	

export function connect()
{
	state = document.getElementById("SA_VBAP-status");
	state.innerText = "connecting...";
	state.style.backgroundColor = "blue";
	sendResponse('/app/SA_VBAP/osc/Control/Response', 9336);
	sendNoArgs('/app/SA_VBAP/osc/Control/Version');
	secWaited = 0;
	setTimeout(() => { checkConnection(); }, 300);
}

export function checkConnection()
{
	const timeout = 1000; // timeout in ms
	secWaited += 100;
	state = document.getElementById("SA_VBAP-Control_Version");
	console.log("waiting for " + secWaited/1000 + "s, version=" + state.innerText);
	switch (state.innerText)
	{
		case "---":
			if (secWaited <= timeout)
			{		// try again
				sendResponse('/app/SA_VBAP/osc/Control/Response');
				sendNoArgs('/app/SA_VBAP/osc/Control/Version');		
				setTimeout(() => { checkConnection(); }, 100);
			}
			break;
		default: // done!
			state = document.getElementById("SA_VBAP-status");
			state.innerText = "connected";
			state.style.backgroundColor = "";
			sendNoArgs('/app/SA_VBAP/osc/Control/SampleRate');
			sendNoArgs('/app/SA_VBAP/osc/Total/Volume');
      sendNoArgs('/app/SA_VBAP/osc/Control/Door');
			break;
	}
	if (secWaited > timeout)
	{ 	// time out -> clean up
		state = document.getElementById("SA_VBAP-status");
		state.innerText = "time out while connecting";
		state.style.backgroundColor = "red";
	}
}		

export function Door_Left()
{
  send('/app/SA_VBAP/osc/Control/Door/Set', 'OO');
}

export function Door_Right()
{
  send('/app/SA_VBAP/osc/Control/Door/Set', 'OC');
}

export function Door_Status()
{
  state = document.getElementById("SA_VBAP-Control_Door");
  switch (state.innerText)
  {
    case "OO":
      document.getElementById("SA_VBAP-Door_Left_Open").innerText = "Open";
      document.getElementById("SA_VBAP-Door_Left_Open").disabled = true;
      document.getElementById("SA_VBAP-Door_Left_Close").innerText = "Close";
      document.getElementById("SA_VBAP-Door_Left_Close").disabled = false;
      document.getElementById("SA_VBAP-Door_Right_Open").innerText = "Open";
      document.getElementById("SA_VBAP-Door_Right_Open").disabled = true;
      document.getElementById("SA_VBAP-Door_Right_Close").innerText = "Close";
      document.getElementById("SA_VBAP-Door_Right_Close").disabled = false;
      break;
    case "OC":
      document.getElementById("SA_VBAP-Door_Left_Open").innerText = "Open";
      document.getElementById("SA_VBAP-Door_Left_Open").disabled = true;
      document.getElementById("SA_VBAP-Door_Left_Close").innerText = "Close";
      document.getElementById("SA_VBAP-Door_Left_Close").disabled = false;
      document.getElementById("SA_VBAP-Door_Right_Open").innerText = "Open";
      document.getElementById("SA_VBAP-Door_Right_Open").disabled = false;
      document.getElementById("SA_VBAP-Door_Right_Close").innerText = "Close";
      document.getElementById("SA_VBAP-Door_Right_Close").disabled = true;
      break;
    case "CO":
      document.getElementById("SA_VBAP-Door_Left_Open").innerText = "Open";
      document.getElementById("SA_VBAP-Door_Left_Open").disabled = false;
      document.getElementById("SA_VBAP-Door_Left_Close").innerText = "Close";
      document.getElementById("SA_VBAP-Door_Left_Close").disabled = true;
      document.getElementById("SA_VBAP-Door_Right_Open").innerText = "Open";
      document.getElementById("SA_VBAP-Door_Right_Open").disabled = true;
      document.getElementById("SA_VBAP-Door_Right_Close").innerText = "Close";
      document.getElementById("SA_VBAP-Door_Right_Close").disabled = false;
      break;
    case "CC":
      document.getElementById("SA_VBAP-Door_Left_Open").innerText = "Open";
      document.getElementById("SA_VBAP-Door_Left_Open").disabled = false;
      document.getElementById("SA_VBAP-Door_Left_Close").innerText = "Close";
      document.getElementById("SA_VBAP-Door_Left_Close").disabled = true;
      document.getElementById("SA_VBAP-Door_Right_Open").innerText = "Open";
      document.getElementById("SA_VBAP-Door_Right_Open").disabled = false;
      document.getElementById("SA_VBAP-Door_Right_Close").innerText = "Close";
      document.getElementById("SA_VBAP-Door_Right_Close").disabled = true;
      break;
    default:
      document.getElementById("SA_VBAP-Door_Left_Open").innerText = "unknown";
      document.getElementById("SA_VBAP-Door_Left_Open").disabled = true;
      document.getElementById("SA_VBAP-Door_Left_Close").innerText = "unknown";
      document.getElementById("SA_VBAP-Door_Left_Close").disabled = true;
      document.getElementById("SA_VBAP-Door_Right_Open").innerText = "unknown";
      document.getElementById("SA_VBAP-Door_Right_Open").disabled = true;
      document.getElementById("SA_VBAP-Door_Right_Close").innerText = "unknown";
      document.getElementById("SA_VBAP-Door_Right_Close").disabled = true;
      break;
  }
}