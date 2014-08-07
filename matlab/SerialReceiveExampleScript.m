%% SerialReceiveExampleScript
serial_handle=serial('COM70', 'BaudRate', 9600,'InputBufferSize',1024,'ReadAsyncMode','continuous');
fopen(serial_handle)
q=Quaternion;

q=SerialReceive(serial_handle)

fclose(serial_handle)