export function init()
{
}
	/* States: 
		Open circle (&#9711;), black: shutdown
		Closed circle (&#11044), black: switched on
		Closed circle, green: unmuted
		Closed circle, blue: muted
		Closed circle, red: error
		Closed circle, yellow: state unknown
	*/

const amps_status = [ 
		document.querySelector('#amp1-status'), 
		document.querySelector('#amp2-status'), 
		document.querySelector('#amp3-status'), 
		document.querySelector('#amp4-status'), 
		document.querySelector('#amp5-status'), 
		document.querySelector('#amp6-status'), 
		document.querySelector('#amp7-status'), 
		document.querySelector('#amp8-status'), ];
		
// Controls the status when running "PDU Start"
document.getElementById('pdu_start-status').addEventListener('updated', (e) => 
{
  const status = document.getElementById('pdu_start-status');
	for (var i=1; i<=8; i++)
	{
		switch (status.innerText)
		{
			case "alive": // script starting, feedback to the user
				amps_status[i-1].innerHTML = "-";
				amps_status[i-1].style.color = "black";
				break;
			case "ready": // script running, devices powering up
				amps_status[i-1].innerHTML = "-";
				amps_status[i-1].style.color = "blue";
				break; 
			case "success": // script done, devices have power
				amps_status[i-1].innerHTML = "&#11044";
				amps_status[i-1].style.color = "black";
		}
	}
});

// Controls the status when running "PDU Shutdown"
document.getElementById('pdu_shutdown-status').addEventListener('updated', (e) => 
{
  const status = document.getElementById('pdu_shutdown-status');
	for (var i=1; i<=8; i++)
	{
		switch (status.innerText)
		{
			case "alive": // script starting, feedback to the user
				amps_status[i-1].innerHTML = "-";
				amps_status[i-1].style.color = "black";
				break;
			case "ready": // script running, devices powering up
				amps_status[i-1].innerHTML = "-";
				amps_status[i-1].style.color = "blue";
				break; 
			case "success": // script done, devices have power
				amps_status[i-1].innerHTML = "&#9711";
				amps_status[i-1].style.color = "black";
		}
	}
});

// Controls the status when running "Amps: Unmute"
document.getElementById('amps_unmute-status').addEventListener('updated', (e) => 
{
  const status = document.getElementById('amps_unmute-status');
	for (var i=1; i<=8; i++)
	{
		switch (status.innerText)
		{
			case "alive": // script starting, feedback to the user
				amps_status[i-1].innerHTML = "-";
				break;
			case "ready": // script running, feedback to the user with the old state
				amps_status[i-1].innerHTML = "&#11044";
				break; 
			case "success": // script done, amps unmuted
				amps_status[i-1].innerHTML = "&#11044";
				amps_status[i-1].style.color = "green";
		}
	}
});

// Controls the status when running "Amps: Mute"
document.getElementById('amps_mute-status').addEventListener('updated', (e) => 
{
  const status = document.getElementById('amps_mute-status');
	for (var i=1; i<=8; i++)
	{
		switch (status.innerText)
		{
			case "alive": // script starting, feedback to the user
				amps_status[i-1].innerHTML = "-";
				break;
			case "ready": // script running, feedback to the user with the old state
				amps_status[i-1].innerHTML = "&#11044";
				break; 
			case "success": // script done, amps muted
				amps_status[i-1].innerHTML = "&#11044";
				amps_status[i-1].style.color = "blue";
		}
	}
});

// Controls the status when running "Amps: Shutdown"
document.getElementById('amps_shutdown-status').addEventListener('updated', (e) => 
{
  const status = document.getElementById('amps_shutdown-status');
	for (var i=1; i<=8; i++)
	{
		switch (status.innerText)
		{
			case "alive": // script starting, feedback to the user
				amps_status[i-1].innerHTML = "-";
				break;
			case "ready": // script running, feedback to the user with the old state
				amps_status[i-1].innerHTML = "&#11044";
				break; 
			case "success": // script done, amps shut down
				amps_status[i-1].innerHTML = "&#11044";
				amps_status[i-1].style.color = "black";
		}
	}
});

// Fetches the status from the amps
/*document.querySelector('#control-Update_Status').addEventListener('click', async () => 
{
	// script starting, feedback to the user
	for (var i=1; i<=8; i++)
		amps_status[i-1].innerHTML = "-";
	
	//setTimeout( () => { updateAmpStatus(); }, 0);
});*/

// Fetches the status from the amps
document.querySelector('#control-Update_Status').addEventListener('click', async () => 
{

  var x = await fetchHttpResponse('http://172.16.60.111/api/get/general/model', 200);
	console.log(x);
	if(x.error)
	{
		alert ("Error happened: " + x.error.name + "\n" + x.error.message + "\n\nIf the error is something with failed to fetch, try to install the add-on 'Allow CORS' and switch it on."); 
		return;
	}
	

	for (var i=1; i<=8; i++)
	{
		x = await fetchHttpResponse('http://172.16.60.11' + i + '/api/get/system/mute', 100);
		if(x.error)
		{
			amps_status[i-1].innerHTML = "&#11044";
			amps_status[i-1].style.color = "red";
		}
		else
		{
			amps_status[i-1].innerHTML = "&#11044";
			switch (x.response)
			{
				case "1": // muted
					amps_status[i-1].style.color = "blue";
					break;
				case "0": // unmuted
					amps_status[i-1].style.color = "green";
					break; 
				default: // unknown
					amps_status[i-1].style.color = "yellow";
			}
		}
	}
	
});
