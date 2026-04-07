export function init()
{
	document.getElementById("reaper_music-status").innerText = "initialized";
	document.getElementById("reaper_music-status").style.backgroundColor = "";
  document.getElementById("reaper_music-btn-play").disabled = true;
  document.getElementById("reaper_music-btn-stop").disabled = true;
  document.getElementById("volume-slider").disabled = true;
  document.getElementById("reaper_music-btn-rewind").disabled = true;

  document.getElementById('reaper_music-stop').addEventListener('updated', (e) => 
  {
    document.getElementById("reaper_music-btn-play").disabled = false;
    document.getElementById("reaper_music-btn-stop").disabled = false;
    document.getElementById("volume-slider").disabled = false;
    document.getElementById("reaper_music-btn-rewind").disabled = false;
  });			

  document.getElementById('reaper_music-track_7_vu').addEventListener('updated', (e) => 
  {
    const vu = document.getElementById('reaper_music-track_7_vu');
    let vudB = 0;
    if(vu.innerText != "0")
      vudB = vu.innerText*100; // if vu linear: Math.log10(vu.innerText)*20+100; 
    document.getElementById('reaper_music-Play_VU-bar').style.height = vudB + '%';
    document.getElementById('reaper_music-Play_VU-number').innerText = (vudB).toFixed(1) + ' dB';
  });			

  document.getElementById('reaper_music-time').addEventListener('updated', (e) => 
  {
    const dur = document.getElementById('reaper_music-time').innerText;
    document.getElementById('timecode').innerText = Math.round(dur / 60) + ":" + (dur % 60).toFixed(1);
  });

}


