export function init()
{
	document.getElementById("Curved_LED_Standard-status").innerText = "initialized";
	document.getElementById("Curved_LED_Standard-status").style.backgroundColor = "";	
	document.getElementById("Curved_LED_Standard-Control_Version").innerText = "---";
	document.getElementById("Curved_LED_Standard-Control_SampleRate").innerText = "---";
}

document.getElementById('Curved_LED_Standard-Total_VU').addEventListener('updated', (e) => 
{
	const vu = document.getElementById('Curved_LED_Standard-Total_VU');
	document.getElementById('Curved_LED_Standard-Total_VU-bar').style.height = vu.innerText + '%';
	document.getElementById('Curved_LED_Standard-Total_VU-number').innerText = Math.round(vu.innerText) + ' dB';
});			

document.getElementById('Curved_LED_Standard-Total_Volume').addEventListener('updated', (e) => 
{
	const vol = document.getElementById('Curved_LED_Standard-Total_Volume');
	document.getElementById('volume-slider').value = Math.round(vol.innerText);
	document.getElementById('volume-number').innerText = Math.round(vol.innerText) + " dB";
});	

export function connect()
{
	state = document.getElementById("Curved_LED_Standard-status");
	state.innerText = "connecting...";
	state.style.backgroundColor = "blue";
	sendResponse('/app/Curved_LED_Standard/osc/Control/Response', 9340);
	sendNoArgs('/app/Curved_LED_Standard/osc/Control/Version');
	secWaited = 0;
	setTimeout(() => { checkConnection(); }, 300);
}

export function checkConnection()
{
	const timeout = 1000; // timeout in ms
	secWaited += 100;
	state = document.getElementById("Curved_LED_Standard-Control_Version");
	console.log("waiting for " + secWaited/1000 + "s, version=" + state.innerText);
	switch (state.innerText)
	{
		case "---":
			if (secWaited <= timeout)
			{		// try again
				sendResponse('/app/Curved_LED_Standard/osc/Control/Response');
				sendNoArgs('/app/Curved_LED_Standard/osc/Control/Version');		
				setTimeout(() => { checkConnection(); }, 100);
			}
			break;
		default: // done!
			state = document.getElementById("Curved_LED_Standard-status");
			state.innerText = "connected";
			state.style.backgroundColor = "";
			sendNoArgs('/app/Curved_LED_Standard/osc/Control/SampleRate');
			sendNoArgs('/app/Curved_LED_Standard/osc/Total/Volume');
			break;
	}
	if (secWaited > timeout)
	{ 	// time out -> clean up
		state = document.getElementById("Curved_LED_Standard-status");
		state.innerText = "time out while connecting";
		state.style.backgroundColor = "red";
	}
}		


export function showInputSection(id)
{
	console.log('showInputSection: ' + id);
	// hide all input sections
	const allSections = document.querySelectorAll('.input-section');
	allSections.forEach(section => { section.style.display = 'none'; });

	// show the selected input section
	const section = document.getElementById('input-section-' + id);
	if (section) section.style.display = 'block';
}

export function toggleInputState(id)
{
	console.log('toggleInputState: ' + id);
	
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
