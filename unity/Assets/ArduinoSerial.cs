using UnityEngine;
using System.Collections;
using System.IO.Ports;
using System.Threading;

public class ArduinoSerial : MonoBehaviour {

	public static SerialPort sp = new SerialPort ("COM70", 9600, Parity.None, 8, StopBits.One);
	public int once = 0;
	public static string strIn;    

	void Start () {}
	
	void Update()
	{
		if (once == 0) {
			if (sp.IsOpen)
				sp.Close ();
			sp.Open ();  // opens the connection
			sp.ReadTimeout = 50;  // sets the timeout value before reporting error
			once = 1;
		}
				//Read incoming data
				if (Input.GetAxis ("Horizontal") > 0) {
						sp.Write ("z");
				} else {
						sp.Write ("!");
				}

	}

	void OnApplicationQuit() 
	{
		sp.Close();
	}
}