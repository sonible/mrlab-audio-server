export function init()
{
}

const btn = document.querySelector('#control-Update_Status');
const display = document.querySelector('#amp1-status');

btn.addEventListener('click', async () => 
{
  const url = 'http://172.16.60.112/api/get/general/model';
  
 try 
  {
    const response = await fetch(url, { signal: AbortSignal.timeout(100) } );
    if (!response.ok) 
	{
      throw new Error(`Error: ${response.status}`);
    }
    display.textContent = await response.text();
  } 
  catch (error) 
  {
	if (error.name === 'TimeoutError') 
	{ 
	  display.textContent = `D`; 
	 }
    else	 
      display.textContent = `Error: ${error.message}`;
  }
});
