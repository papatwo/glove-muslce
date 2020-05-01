# glove-muslce
To compile, run `python3 ard_main_test.py`
To test sender Arduino status, run `python3 ard_sub_test.py`

`ard_main_test.py`:	main function to open receiver port (muscle) and transmit data

`ard_sub_test.py`:	subprocess to open sender port (glove)

`mega_serial`: Arduino code for reading sensor values to Arduino

`subard_48port`: Arduino code for receiving pre-processed data to control muscle

utils script:
`calibration2.py`
`readdata.py`
`sensor_sercom2.py`	
`serial_thread_ltc1660.py`
