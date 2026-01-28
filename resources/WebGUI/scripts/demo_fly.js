export function init()
{
	document.getElementById("pd_fly-status").innerText = "initialized";
	document.getElementById("pd_fly-status").style.backgroundColor = "";	
	document.getElementById("pd_fly-Control_Version").innerText = "---";
	document.getElementById("pd_fly-Control_SampleRate").innerText = "---";
}

document.getElementById('pd_fly-Play_VU').addEventListener('updated', (e) => 
{
	document.getElementById('pd_fly-Play_VU-bar').style.height = document.getElementById('pd_fly-Play_VU').innerText + '%';
	console.log('level received:' + document.getElementById('pd_fly-status').innerText + '%');
});			


export function connect()
{
	state = document.getElementById("pd_fly-status");
	state.innerText = "connecting...";
	state.style.backgroundColor = "blue";
	sendResponse('/app/pd_fly/osc/Control/Response', 9336);
	sendNoArgs('/app/pd_fly/osc/Control/Version');
	secWaited = 0;
	setTimeout(() => { checkConnection(); }, 100);
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
				sendResponse('/app/pd_fly/osc/Control/Response');
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
			break;
	}
	if (secWaited > timeout)
	{ 	// time out -> clean up
		state = document.getElementById("pd_fly-status");
		state.innerText = "time out while connecting";
		state.style.backgroundColor = "red";
	}
}		

