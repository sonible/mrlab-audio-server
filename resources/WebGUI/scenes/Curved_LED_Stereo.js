import { renderInputControls, showInputSection, toggleInputState, enableInputSelectButtons } from '../scripts/input-controls.js';
export { showInputSection, toggleInputState, enableInputSelectButtons };

export function init()
{
	document.getElementById("Curved_LED_Stereo-status").innerText = "initialized";
	document.getElementById("Curved_LED_Stereo-status").style.backgroundColor = "";	
  
    // Inject input controls and disabled buttons
   renderInputControls('input-buttons-container', 'input-group-container', 'Curved_LED_Stereo', 
      ["DANTE_CurvedLEDPC", "DANTE_CurvedLEDPC_Channel_3", "DANTE_Mobile", "Mic_Array"]);
  enableInputSelectButtons(false);

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
    
}

export function Lock()
{
  lockScene('Curved_LED_Stereo');
  sendNoArgs('/matrix/settings/flex_channel/*/mute');
  sendNoArgs('/matrix/settings/sum_bus_master/6/gain');
  sendNoArgs('/matrix/settings/sum_bus_master/7/gain');
  enableInputSelectButtons(true);
  document.getElementById("volume-slider").disabled = false;
}

export function Unlock()
{
  sendValue('/matrix/settings/flex_channel/*/mute', 1);
  unlockScene('Curved_LED_Stereo');
  enableInputSelectButtons(false);
  document.getElementById("volume-slider").disabled = true;
}
