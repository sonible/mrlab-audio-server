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
			
function sendResponse(address) 
{
	oscPort.send({
		address: address,
		args: [
			{ type: "s", value: "connect" },
			{ type: "s", value: "localhost" },
			{ type: "i", value: 9336 }
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
	console.log("OSC message received: ", oscMsg);
	pathstr= oscMsg.address.substring(1);
	path = pathstr.split("/");
		// Format of the message: device/scenename/osc/command1/command2/.../commandN
	switch (path[0])
	{
		case 'app': // Messages related to an app
			switch (path[2])
			{
				case 'state': // Launch status received 
					document.getElementById(path[1] + "_status").innerHTML = oscMsg.args[1].value;
					break;
	
				case 'osc': // OSC message from the app received
					commands = (pathstr.substring(path[0].length+path[1].length+path[2].length+3));
						// Format of the variable: scenename-command1-command2-...-commandN
					document.getElementById(path[1] + "_" + commands.replace("/", "-")).innerHTML = oscMsg.args[0].value;
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
