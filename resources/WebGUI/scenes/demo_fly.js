export function init()
{
	document.getElementById("pd_fly-status").innerText = "initialized";
	document.getElementById("pd_fly-status").style.backgroundColor = "";	
	document.getElementById("pd_fly-Control_Version").innerText = "---";
	document.getElementById("pd_fly-Control_SampleRate").innerText = "---";
  document.getElementById("pd_fly-Play_Start").disabled = true;
  document.getElementById("pd_fly-Play_Stop").disabled = true;
  document.getElementById("volume-slider").disabled = true;

  document.getElementById('pd_fly-Play_VU').addEventListener('updated', (e) => 
  {
    const vu = document.getElementById('pd_fly-Play_VU');
    document.getElementById('pd_fly-Play_VU-bar').style.height = vu.innerText + '%';
    document.getElementById('pd_fly-Play_VU-number').innerText = Math.round(vu.innerText) + ' dB';
  });			

  document.getElementById('pd_fly-Play_Volume').addEventListener('updated', (e) => 
  {
    const vol = document.getElementById('pd_fly-Play_Volume');
    document.getElementById('volume-slider').value = Math.round(vol.innerText);
    document.getElementById('volume-number').innerText = Math.round(vol.innerText) + " dB";
  });	

  document.getElementById('pd_fly-Play_Duration').addEventListener('updated', (e) => 
  {
    const dur = document.getElementById('pd_fly-Play_Duration').innerText;
    document.getElementById('duration').innerText = Math.round(dur / 60) + ":" + (dur % 60).toFixed(1);
  });

  document.getElementById('pd_fly-Play_Timecode').addEventListener('updated', (e) => 
  {
    const dur = document.getElementById('pd_fly-Play_Timecode').innerText;
    document.getElementById('timecode').innerText = Math.round(dur / 60) + ":" + (dur % 60).toFixed(1);
  });


}

export function connect()
{
	state = document.getElementById("pd_fly-status");
	state.innerText = "connecting...";
	state.style.backgroundColor = "blue";
	sendResponse('/app/pd_fly/osc/Control/Response', 9336);
	sendNoArgs('/app/pd_fly/osc/Control/Version');
	secWaited = 0;
	setTimeout(() => { checkConnection(); }, 300);
}

export function checkConnection()
{
	const timeout = 1000; // timeout in ms
	secWaited += 100;
	state = document.getElementById("pd_fly-Control_Version");
	console.log("waiting for " + secWaited/1000 + "s, version=" + state.innerText);
	switch (state.innerText)
	{
		case "---":
			if (secWaited <= timeout)
			{		// try again
				sendResponse('/app/pd_fly/osc/Control/Response', 9336);
				sendNoArgs('/app/pd_fly/osc/Control/Version');		
				setTimeout(() => { checkConnection(); }, 100);
			}
			break;
		default: // done!
			state = document.getElementById("pd_fly-status");
			state.innerText = "connected";
			state.style.backgroundColor = "";
			sendNoArgs('/app/pd_fly/osc/Control/SampleRate');
			sendNoArgs('/app/pd_fly/osc/Play/Volume');
			sendNoArgs('/app/pd_fly/osc/Play/Duration');
      document.getElementById("pd_fly-Play_Start").disabled = false;
      document.getElementById("pd_fly-Play_Stop").disabled = false;
      document.getElementById("volume-slider").disabled = false;
			break;
	}
	if (secWaited > timeout)
	{ 	// time out -> clean up
		state = document.getElementById("pd_fly-status");
		state.innerText = "time out while connecting";
		state.style.backgroundColor = "red";
	}
}		

