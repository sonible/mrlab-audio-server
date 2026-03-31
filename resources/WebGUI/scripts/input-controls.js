
const inputNames = [
    "DANTE_CurvedLEDPC",
    "DANTE_Bluetooth", 
    "DANTE_HDMI_Stereo", 
    "DANTE_Mobile", 
    "Beam_Mic_Curved", "Beam_Mic_CAVE", "Mic_Wireless_1", "Mic_Wireless_2", "Mic_Array",
    "Analog_Mono_1", "Analog_Mono_2", "Analog_Mono_3", "Analog_Mono_4",
    "Analog_Mono_5", "Analog_Mono_6", "Analog_Mono_7", "Analog_Mono_8",
    "Analog_Stereo_1", "Analog_Stereo_2", "Analog_Stereo_3", "Analog_Stereo_4", 
    "DANTE_Mono_1", "DANTE_Mono_2", "DANTE_Mono_3", "DANTE_Mono_4",
    "DANTE_Mono_5", "DANTE_Mono_6", "DANTE_Mono_7", "DANTE_Mono_8",
    "DANTE_Stereo_1", "DANTE_Stereo_2", "DANTE_Stereo_3", "DANTE_Stereo_4",
];

const inputLabels = {
    "DANTE_CurvedLEDPC": "Curved LED PC Stereo",
    "DANTE_Bluetooth": "Bluetooth Stereo",
    "DANTE_HDMI_Stereo": "HDMI Stereo", 
    "DANTE_Mobile": "Mobile Dante Station", 
    "DANTE_Mono_1": "Dante: Mono #1", "DANTE_Mono_2": "Dante: Mono #2",
    "DANTE_Mono_3": "Dante: Mono #3", "DANTE_Mono_4": "Dante: Mono #4",
    "DANTE_Mono_5": "Dante: Mono #5", "DANTE_Mono_6": "Dante: Mono #6",
    "DANTE_Mono_7": "Dante: Mono #7", "DANTE_Mono_8": "Dante: Mono #8",
    "DANTE_Stereo_1": "Dante: Stereo #1", "DANTE_Stereo_2": "Dante: Stereo #2",
    "DANTE_Stereo_3": "Dante: Stereo #3", "DANTE_Stereo_4": "Dante: Stereo #4",
    "Beam_Mic_Curved": "Beam Mic Curved", "Beam_Mic_CAVE": "Beam Mic CAVE",
    "Mic_Wireless_1": "Wireless Mic #1", "Mic_Wireless_2": "Wireless Mic #2", 
    "Mic_Array": "Ceiling Mic Array",
    "Analog_Mono_1": "Analog: Mono #1", "Analog_Mono_2": "Analog: Mono #2",
    "Analog_Mono_3": "Analog: Mono #3", "Analog_Mono_4": "Analog: Mono #4",
    "Analog_Mono_5": "Analog: Mono #5", "Analog_Mono_6": "Analog: Mono #6",
    "Analog_Mono_7": "Analog: Mono #7", "Analog_Mono_8": "Analog: Mono #8",
    "Analog_Stereo_1": "Analog: Stereo #1", "Analog_Stereo_2": "Analog: Stereo #2",
    "Analog_Stereo_3": "Analog: Stereo #3", "Analog_Stereo_4": "Analog: Stereo #4"
};


export function renderInputControls(buttonContainerId, groupContainerId, sceneName, hiddenInputs = []) {
    const btnContainer = document.getElementById(buttonContainerId);
    if (btnContainer) {
        let html = '';
        html += '<div style="max-height: 350px; overflow-y: auto; display: flex; flex-direction: column; align-items: center; gap: 10px;">';
        inputNames.forEach(id => {
             if (!hiddenInputs.includes(id)) {
                 html += `<button id="btn-${id}" onClick="SceneModule.showInputSection('${id}')" class="big-button">${inputLabels[id]}</button> `;
             }
        });
        html += '</div>';
        btnContainer.innerHTML = html;
    }

    const groupContainer = document.getElementById(groupContainerId);
    if (groupContainer) {
        let html = '';
        inputNames.forEach(id => {
            if (!hiddenInputs.includes(id)) {
                html += `
                <div id="input-section-${id}" class="input-section" style="display:none; flex-direction: column; align-items: center; text-align: center;">
                    <span style="text-align: center; font-weight: bold; gap: 0px;">${inputLabels[id]}</span>
                    <button id="btn-input-${id}" style="margin-top: 10px;" class="small-button" onclick="SceneModule.toggleInputState('${id}')">Input: Off</button>
                    <div id="slider-container-${id}">
                        <input class="volume-slider" type="range" id="slider-input-${id}" disabled min="0" max="99" value="0" 
                            oninput="sendValue('/app/${sceneName}/osc/Input/${id}/Volume/Set', this.value); document.getElementById('volume-number-${id}').innerText = this.value + ' dB';" 
                            style="opacity: 0.5; max-height: 220px;">
                        <div class="level-box" id="volume-number-${id}">--</div>
                    </div>
                </div>`;
            }
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
	if (section) section.style.display = 'flex';
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

export function renderInputButtons(buttonContainerId, sceneName, hiddenInputs = []) {
    const btnContainer = document.getElementById(buttonContainerId);
    if (btnContainer) {
        let html = '';
        html += '<div style="max-height: 350px; overflow-y: auto; display: flex; flex-direction: column; align-items: center; gap: 10px;">';
        inputNames.forEach(id => {
             if (!hiddenInputs.includes(id)) {
                 html += `<button id="btn-${id}" onClick="SceneModule.setInputButtonExclusively('${id}', '${sceneName}')" class="big-button">${inputLabels[id]}</button> `;
             }
        });
        html += '</div>';
        btnContainer.innerHTML = html;
    }
}

export function setInputButtonExclusively(id, sceneName)
{	
	const bigBtn = document.getElementById('btn-' + id);
	if (!bigBtn) return;

	// Check if currently active by class
	const isActive = bigBtn.classList.contains('active-input');

	if (!isActive) // User wants to turn it ON
	{
		// Find any currently active simple buttons
		inputNames.forEach(otherId => {
			if (otherId === id) return;
			const otherBtn = document.getElementById('btn-' + otherId);
			if (otherBtn && otherBtn.classList.contains('active-input')) {
				otherBtn.classList.remove('active-input');
				sendValue(`/app/${sceneName}/osc/Input/${otherId}/Volume/Set`, 0);
			}
		});

		bigBtn.classList.add('active-input');
		sendValue(`/app/${sceneName}/osc/Input/${id}/Volume/Set`, 100); 
	}
	else // Turn OFF
	{
		bigBtn.classList.remove('active-input');
		sendValue(`/app/${sceneName}/osc/Input/${id}/Volume/Set`, 0);
	}
}
