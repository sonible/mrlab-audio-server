# WebGUI

Buttons on the left side load a Scene. Technically, each button loads an HTML file `scenes/%SceneName%.html` and optionally a Javascript module `scripts/%SceneName%.js`.

## A Scene

* Must start with `<div id="%SceneName%" class="scene-main">` and end with `</div>`.

* No scripts allowed. If scripts are required: 
  * Create a JS file in `scripts/%SceneName%.js`.
	* Use `export function` instead of `function`, 
	* Include a function called `init`. This function is obligatory and will be executed before the scene will be shown. 
	* Functions within that script can be accessed as `SceneModule.%someFunction%`, e.g., `SceneModule.init()`. 

* The highest level of header must be `<h2>%SceneName%</h2>`.

* To start an app: ` <button onClick="launchApp(%SceneName%, %appName%)">Launch App</button>`. Server responses with the following states: 
  * `alive`: command acknowledged, launching the app.
  * `ready`: 3 seconds after `alive`. App is assumed to be launched, commands can be sent to the app.

* To start an app and wait until the app has been launched: ` <button onClick="launchAppAndWait(%SceneName%, %appName%, %timeout%)">Launch App</button>`, which waits %timeout% seconds before creating an error. 

* To start an app, wait, and do some processing: ` <button onClick="launchAppAndWait(%SceneName%, %appName%, %timeout%, SceneModule.init, SceneModule.ready)">Launch App</button>`, which calls the scene-related `init()` function before launching the `%appName%` and `ready()` after the app has been successfully launched. 

* To quit an app: ` <button onClick="quitApp(%SceneName%, %appName%)">Quit App</button>. Server responses with the following states: 
  * `quitting...`: command acknowledged, quitting the app.
  * `killed`: app killed.
  * `error`: app returns exit code <> 0 on exit. 
  * `success`: app returns exit code 0 on exit.

* To quit an app and execute a function right after the quit command has been sent to the server: `quitApp(%SceneName%, %appName%, SceneModule.%quitFunction%)`.

* To connect to the app for a response channel: ` <button onClick="sendResponse('/app/%appName%/%pathToEstablishTheResponse%')">Establish Response</button> `.

* To send a command: ` <button onClick="sendNoArgs('/app/%appName%/%command%')">Send %command%</button> `.

* To send a command with a value: `<input type=range oninput="send('/app/%appName%/osc/%command%', this.value)">`

* To receive a state of a %variable% from the app, create an element such as: ` <span id="%appName%_%variable%"></span> `. The innertext of this variable will be updated each time, when an OSC message with %command% == %variable% will be received. Note that all "/" in %command% will be translated to "-" in %variable%, e.g., for a %command% of `Control/SamplingRate`, the %variable% must be `Control-SamplingRate`. 

* To receive the state of an app being loaded or killed, receive the state of the variable called `%appname%-status`.