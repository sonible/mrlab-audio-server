var oscPort = new osc.WebSocketPort({
		url: "/ws", // WebSocket resource at current URL context.
		metadata: true
});
oscPort.open();

function send(address, value) 
{
	oscPort.send({ 
		address: address,
		args: [{ type: "s", value: value }]
	});
}

function sendValue(address, value) 
{
	oscPort.send({
		address: address,
		args: [{ type: "f", value: value }]
	});
}
			
function sendResponse(address,port) 
{
	oscPort.send({
		address: address,
		args: [
			{ type: "s", value: "connect" },
			{ type: "s", value: "localhost" },
			{ type: "i", value: port }
		]
	});
}

function sendNoArgs(address) 
{
	oscPort.send({ address: address });
}

oscPort.on("ready", function () 
{
	oscPort.send({ address: "/ping" });
});

oscPort.on("message", function (oscMsg) 
{
	// console.log("OSC message received: ", oscMsg);
	pathstr= oscMsg.address.substring(1);
	path = pathstr.split("/");
		// Format of the message: device/scenename/osc/command1/command2/.../commandN
	switch (path[0])
	{
		case 'app': // Messages related to an app
			switch (path[2])
			{
				case 'state': // Launch status received 
					const el = document.getElementById(path[1] + "-status");
					el.innerHTML = oscMsg.args[1].value;
					const event = new CustomEvent('updated', { detail: { time: Date.now() } });
					el.dispatchEvent(event);
					break;
	
				case 'osc': // OSC message from the app received
					commands = (pathstr.substring(path[0].length+path[1].length+path[2].length+3));
						// Format of the variable as scenename-command1_command2_..._commandN
					document.getElementById(path[1] + "-" + commands.replace("/", "_")).innerHTML = oscMsg.args[0].value;
					break;
			};
			break;

		case 'matrix': 
			console.log("Matrix message received");
			break;
			
		default:
			console.log("No OSC message handler: ", oscMsg.address);
			break;
	}
});
