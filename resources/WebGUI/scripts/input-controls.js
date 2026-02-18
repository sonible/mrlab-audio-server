
const inputNames = [
    "DANTE_Mono_1", "DANTE_Mono_2", "DANTE_Mono_3", "DANTE_Mono_4",
    "DANTE_Mono_5", "DANTE_Mono_6", "DANTE_Mono_7", "DANTE_Mono_8",
    "DANTE_Stereo_1", "DANTE_Stereo_2", "DANTE_Stereo_3", "DANTE_Stereo_4",
    "DANTE_Bluetooth", "DANTE_HDMI", "DANTE_GCPU", "DANTE_CurvedLEDPC",
    "Mic_Beam_1", "Mic_Beam_2", "Mic_Wireless_1", "Mic_Wireless_2", "Mic_Array",
    "Analog_Mono_1", "Analog_Mono_2", "Analog_Mono_3", "Analog_Mono_4",
    "Analog_Mono_5", "Analog_Mono_6", "Analog_Mono_7", "Analog_Mono_8",
    "Analog_Stereo_1", "Analog_Stereo_2", "Analog_Stereo_3", "Analog_Stereo_4"
];

const inputLabels = {
    "DANTE_Mono_1": "DANTE: Mono #1", "DANTE_Mono_2": "DANTE: Mono #2",
    "DANTE_Mono_3": "DANTE: Mono #3", "DANTE_Mono_4": "DANTE: Mono #4",
    "DANTE_Mono_5": "DANTE: Mono #5", "DANTE_Mono_6": "DANTE: Mono #6",
    "DANTE_Mono_7": "DANTE: Mono #7", "DANTE_Mono_8": "DANTE: Mono #8",
    "DANTE_Stereo_1": "DANTE: Stereo #1", "DANTE_Stereo_2": "DANTE: Stereo #2",
    "DANTE_Stereo_3": "DANTE: Stereo #3", "DANTE_Stereo_4": "DANTE: Stereo #4",
    "DANTE_Bluetooth": "DANTE: Bluetooth",
    "DANTE_HDMI": "DANTE: HDMI", "DANTE_GCPU": "DANTE: GCPU",
    "DANTE_CurvedLEDPC": "DANTE: CurvedLEDPC",
    "Mic_Beam_1": "Mic: Beam #1", "Mic_Beam_2": "Mic: Beam #2",
    "Mic_Wireless_1": "Mic: Wireless #1", "Mic_Wireless_2": "Mic: Wireless #2", 
    "Mic_Array": "Mic: Array",
    "Analog_Mono_1": "Analog: Mono #1", "Analog_Mono_2": "Analog: Mono #2",
    "Analog_Mono_3": "Analog: Mono #3", "Analog_Mono_4": "Analog: Mono #4",
    "Analog_Mono_5": "Analog: Mono #5", "Analog_Mono_6": "Analog: Mono #6",
    "Analog_Mono_7": "Analog: Mono #7", "Analog_Mono_8": "Analog: Mono #8",
    "Analog_Stereo_1": "Analog: Stereo #1", "Analog_Stereo_2": "Analog: Stereo #2",
    "Analog_Stereo_3": "Analog: Stereo #3", "Analog_Stereo_4": "Analog: Stereo #4"
};


export function renderInputControls(buttonContainerId, groupContainerId, sceneName) {
    const btnContainer = document.getElementById(buttonContainerId);
    if (btnContainer) {
        let html = '';
        html += '<div style="max-height: 650px; overflow-y: auto; display: flex; flex-direction: column; align-items: center; gap: 10px;">';
        inputNames.forEach(id => {
             html += `<button id="btn-${id}" onClick="SceneModule.showInputSection('${id}')" class="big-button">${inputLabels[id]}</button> `;
        });
        html += '</div>';
        btnContainer.innerHTML = html;
    }

    const groupContainer = document.getElementById(groupContainerId);
    if (groupContainer) {
        let html = '';
        inputNames.forEach(id => {
            html += `
            <div id="input-section-${id}" class="input-section" style="display:none; text-align: center;">
                <h3>${inputLabels[id]}</h3>
                <button id="btn-input-${id}" class="small-button" onclick="SceneModule.toggleInputState('${id}')">Input: Off</button>
                <br><br>
                <div id="slider-container-${id}" class="slider-group" style="width: 100%;">
                    <span>Volume</span><br>
                    <input type="range" id="slider-input-${id}" disabled style="opacity: 0.5; max-height: 420px;" min="0" max="100" value="0" 
                        oninput="sendValue('/app/${sceneName}/osc/Input/${id}/Volume/Set', this.value); document.getElementById('volume-number-${id}').innerText = this.value + ' dB';" 
                        class="volume-slider">
                    <br>
                    <div class="level-box" id="volume-number-${id}">--</div>
                </div>
            </div>`;
        });
        groupContainer.innerHTML = html;
    }
}

export function showInputSection(id)
{
	// hide all input sections
	const allSections = document.querySelectorAll('.input-section');
	allSections.forEach(section => { section.style.display = 'none'; });

	// show the selected input section
	const section = document.getElementById('input-section-' + id);
	if (section) section.style.display = 'block';
}

export function toggleInputState(id)
{	
	const smallBtn = document.getElementById('btn-input-' + id);
	const bigBtn = document.getElementById('btn-' + id);
	const slider = document.getElementById('slider-input-' + id);

	if (!smallBtn || !bigBtn || !slider) return;

	// Check if currently active by class
	const isActive = smallBtn.classList.contains('active-input');

	if (!isActive) // User wants to turn it ON
	{
		smallBtn.classList.add('active-input');
		smallBtn.innerText = "Input: On";
		
		bigBtn.classList.add('active-input');
		
		slider.disabled = false;
		slider.style.opacity = "1.0";
	}
	else // Turn OFF
	{
		smallBtn.classList.remove('active-input');
		smallBtn.innerText = "Input: Off";

		bigBtn.classList.remove('active-input');

		slider.disabled = true;
		slider.style.opacity = "0.5";
	}
}
