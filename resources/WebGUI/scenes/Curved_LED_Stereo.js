export function init()
{
    // Inject input controls and disabled buttons
   renderInputControls('input-buttons-container', 'input-group-container', 'Curved_LED_Stereo', 
      ["DANTE_CurvedLEDPC", "DANTE_CurvedLEDPC_Channel_3", "DANTE_Mobile", "Mic_Array", 
        "DANTE_Mobile_Stereo", // to be setup later
        "DANTE_HDMI_Stereo", // to be setup later
        "DANTE_Bluetooth", // to be setup later
        ]);
  enableInputSelectButtons(false);

  document.getElementById('sum_bus_master-meter').addEventListener('updated', (e) => 
  {
    const vu = document.getElementById('sum_bus_master-meter');
    document.getElementById('sum_bus_master-meter-bar').style.height = vu.innerText + '%';
    document.getElementById('sum_bus_master-meter-number').innerText = Math.round(vu.innerText) + ' dB';
  });

  document.getElementById('sum_bus_master-gain').addEventListener('updated', (e) => 
  {
    const vol = document.getElementById('sum_bus_master-gain');
    console.log(vol.innerText);
    document.getElementById('sum_bus_master-volume-slider').value = Math.round(vol.innerText);
    document.getElementById('sum_bus_master-volume-number').innerText = Math.round(vol.innerText) + ' dB';
  });	
    
}

export function Lock()
{
  lockScene('Curved_LED_Stereo');
  sendNoArgs('/matrix/settings/flex_channel/*/mute');
  sendNoArgs('/matrix/settings/sum_bus_master/0/gain');
  sendNoArgs('/matrix/settings/sum_bus_master/1/gain');
  sendValue('/matrix/settings/sum_bus_master/0/mute', 0); // unmute in case that muted
  sendValue('/matrix/settings/sum_bus_master/1/mute', 0);
  enableInputSelectButtons(true);
  document.getElementById("sum_bus_master-volume-slider").disabled = false;
  setTimeout(() => { triggerMeter(); }, 100);
}

function triggerMeter()
{
  sendNoArgs('/matrix/settings/sum_bus_master/0/meter');
  if (!document.getElementById("sum_bus_master-volume-slider").disabled)
    setTimeout(() => { triggerMeter(); }, 100);
}

export function Unlock()
{
  unlockScene('Curved_LED_Stereo');
  enableInputSelectButtons(false);
  showInputSection('none'); // Hide all input sections
  document.getElementById("sum_bus_master-volume-slider").disabled = true;
}

export function UndlockAndMute()
{
  sendValue('/matrix/settings/flex_channel/*/mute', 1);
  unlockScene('Curved_LED_Stereo');
  enableInputSelectButtons(false);
  showInputSection('none'); // Hide all input sections
  document.getElementById("sum_bus_master-volume-slider").disabled = true;
}