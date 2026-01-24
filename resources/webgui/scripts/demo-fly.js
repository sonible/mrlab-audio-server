export function init()
{
	document.getElementById("pd-fly-status").innerText = "initialized";
	document.getElementById("pd-fly-status").style.backgroundColor = "";	
	document.getElementById("pd-fly_Control-Version").innerText = "---";
	document.getElementById("pd-fly_Control-SampleRate").innerText = "---";
	document.getElementById("pd-fly_Control-CPULoad").innerText = "---";
}

export function connect()
{
	state = document.getElementById("pd-fly-status");
	state.innerText = "connecting...";
	state.style.backgroundColor = "blue";
	sendResponse('/app/pd-fly/osc/Control/Response');
	sendNoArgs('/app/pd-fly/osc/Control/Version');
	secWaited = 0;
	setTimeout(() => { checkConnection(); }, 100);
}

export function checkConnection()
{
	const timeout = 1000; // timeout in ms
	secWaited += 100;
	state = document.getElementById("pd-fly_Control-Version");
	console.log("waiting for " + secWaited/1000 + "s, version=" + state.innerText);
	switch (state.innerText)
	{
		case "---":
			if (secWaited <= timeout)
			{		// try again
				sendResponse('/app/pd-fly/osc/Control/Response');
				sendNoArgs('/app/pd-fly/osc/Control/Version');		
				setTimeout(() => { checkConnection(); }, 100);
			}
			break;
		default: // done!
			state = document.getElementById("pd-fly-status");
			state.innerText = "connected";
			state.style.backgroundColor = "";
			sendNoArgs('/app/pd-fly/osc/Control/CPULoad');		
			sendNoArgs('/app/pd-fly/osc/Control/SampleRate');		
			break;
	}
	if (secWaited > timeout)
	{ 	// time out -> clean up
		state = document.getElementById("pd-fly-status");
		state.innerText = "time out while connecting";
		state.style.backgroundColor = "red";
	}
}		

