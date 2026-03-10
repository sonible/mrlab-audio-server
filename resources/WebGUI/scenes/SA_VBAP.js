import { renderInputButtons, setInputButtonExclusively } from '../scripts/input-controls.js';
export { setInputButtonExclusively };

let state_curved;
let state_cave;

export function init()
{
	document.getElementById("SA_VBAP-status").innerText = "initialized";
	document.getElementById("SA_VBAP-status").style.backgroundColor = "";	
	document.getElementById("SA_VBAP-Control_Version").innerText = "---";
	document.getElementById("SA_VBAP-Control_SampleRate").innerText = "---"; 
	document.getElementById("SA_VBAP-Control_Door").innerText = "---";
    // Inject input controls
    renderInputButtons('input-buttons-container', 'SA_VBAP', 
      [
      "DANTE_Bluetooth", 
      "DANTE_HDMI_Stereo", 
      "Beam_Mic_Curved", "Beam_Mic_CAVE", "Mic_Wireless_1", "Mic_Wireless_2", "Mic_Array",
      "Analog_Mono_1", "Analog_Mono_2", "Analog_Mono_3", "Analog_Mono_4",
      "Analog_Mono_5", "Analog_Mono_6", "Analog_Mono_7", "Analog_Mono_8",
      "Analog_Stereo_1", "Analog_Stereo_2", "Analog_Stereo_3", "Analog_Stereo_4", 
      "DANTE_Mono_1", "DANTE_Mono_2", "DANTE_Mono_3", "DANTE_Mono_4",
      "DANTE_Mono_5", "DANTE_Mono_6", "DANTE_Mono_7", "DANTE_Mono_8",
      "DANTE_Stereo_1", "DANTE_Stereo_2", "DANTE_Stereo_3", "DANTE_Stereo_4"]);

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

  document.getElementById('SA_VBAP-Control_Door').addEventListener('updated', (e) => 
  {
    Door_Status();
    //console.log("Door status: " + document.getElementById("SA_VBAP-Control_Door").innerText);
  });	

}

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
	//console.log("waiting for " + secWaited/1000 + "s, version=" + state.innerText);
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
      console.log("SA_VBAP: connected");
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

export function Door_Change({ state_cave = undefined, state_curved =  undefined })
{
  if(state_curved === undefined)
    state_curved = document.getElementById("SA_VBAP-Door_Curved_Close").disabled;
  if(state_cave === undefined)
    state_cave = document.getElementById("SA_VBAP-Door_CAVE_Close").disabled;

  if(state_cave) state = 'C'; else state = 'O';
  if(state_curved) state = `${state}C`; else state = `${state}O`;

  send('/app/SA_VBAP/osc/Control/Door/Set', state);
  sendNoArgs('/app/SA_VBAP/osc/Control/Door');

  document.getElementById("SA_VBAP-Door_Curved_Open").disabled = true;
  document.getElementById("SA_VBAP-Door_Curved_Close").disabled = true;
  document.getElementById("SA_VBAP-Door_CAVE_Open").disabled = true;
  document.getElementById("SA_VBAP-Door_CAVE_Close").disabled = true;
}

export function Door_Status()
{
  state = document.getElementById("SA_VBAP-Control_Door");
  switch (state.innerText)
  {
    case "OO":
      document.getElementById("SA_VBAP-Door_Curved_Open").disabled = true;
      document.getElementById("SA_VBAP-Door_Curved_Close").disabled = false;
      document.getElementById("SA_VBAP-Door_Curved").innerText = "Door is open";
      document.getElementById("SA_VBAP-Door_CAVE_Open").disabled = true;
      document.getElementById("SA_VBAP-Door_CAVE_Close").disabled = false;
      document.getElementById("SA_VBAP-Door_CAVE").innerText = "Door is open";
      break;
    case "CO":
      document.getElementById("SA_VBAP-Door_Curved_Open").disabled = true;
      document.getElementById("SA_VBAP-Door_Curved_Close").disabled = false;
      document.getElementById("SA_VBAP-Door_Curved").innerText = "Door is open";
      document.getElementById("SA_VBAP-Door_CAVE_Open").disabled = false;
      document.getElementById("SA_VBAP-Door_CAVE_Close").disabled = true;
      break;
    case "OC":
      document.getElementById("SA_VBAP-Door_Curved_Open").disabled = false;
      document.getElementById("SA_VBAP-Door_Curved_Close").disabled = true;
      document.getElementById("SA_VBAP-Door_Curved").innerText = "Door is closed";
      document.getElementById("SA_VBAP-Door_CAVE_Open").disabled = true;
      document.getElementById("SA_VBAP-Door_CAVE_Close").disabled = false;
      document.getElementById("SA_VBAP-Door_CAVE").innerText = "Door is open";
      break;
    case "CC":
      document.getElementById("SA_VBAP-Door_Curved_Open").disabled = false;
      document.getElementById("SA_VBAP-Door_Curved_Close").disabled = true;
      document.getElementById("SA_VBAP-Door_Curved").innerText = "Door is closed";
      document.getElementById("SA_VBAP-Door_CAVE_Open").disabled = false;
      document.getElementById("SA_VBAP-Door_CAVE_Close").disabled = true;
      document.getElementById("SA_VBAP-Door_CAVE").innerText = "Door is closed";
      break;
    default:
      document.getElementById("SA_VBAP-Door_Curved_Open").disabled = true;
      document.getElementById("SA_VBAP-Door_Curved_Close").disabled = true;
      document.getElementById("SA_VBAP-Door_Curved").innerText = "Door is unknown";
      document.getElementById("SA_VBAP-Door_CAVE_Open").disabled = true;
      document.getElementById("SA_VBAP-Door_CAVE_Close").disabled = true;
      document.getElementById("SA_VBAP-Door_CAVE").innerText = "Door is unknown";
      break;
  }
}