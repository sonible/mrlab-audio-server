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

