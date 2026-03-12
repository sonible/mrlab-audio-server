# TotalMix OSC Control

## TotalMix Settings
- Enable OSC Control via "Options" -> "Enable Osc Control"
- Go to OSC settings via "Options" -> "Settings" -> "OSC"
- Make sure incoming and outgoing ports are 7001 and 9001 respectively
- Enter the controlling devices IP in "Remote Controller Address" -> "IP or HostName"

## Loading snapshots via OSC commands
From the webUI, you can now send the following commands **to the MRLabAudioServer**. It will forward the commands to TotalMix internally.
Beware: You really load snapshot "1", by calling the command "../8/1". The numbering is inverted for some reason. These are not a typos.
All commands are sent with a float payload of 1.0.

- load snapshot 1: `/totalmix/3/snapshots/8/1 1.0`
- load snapshot 2: `/totalmix/3/snapshots/7/1 1.0`
- load snapshot 3: `/totalmix/3/snapshots/6/1 1.0`
- load snapshot 4: `/totalmix/3/snapshots/5/1 1.0`
- load snapshot 5: `/totalmix/3/snapshots/4/1 1.0`
- load snapshot 6: `/totalmix/3/snapshots/3/1 1.0`
- load snapshot 7: `/totalmix/3/snapshots/2/1 1.0`
- load snapshot 8: `/totalmix/3/snapshots/1/1 1.0`