%% MainImuScript
serial_handle=serial('COM70', 'BaudRate', 9600,'InputBufferSize',1024,'ReadAsyncMode','continuous');
fopen(serial_handle)
q=Quaternion;

for i=0:2
    q=SerialReceive(serial_handle);
    [psi,theta,phi]=QuaternionToEuler(q.q0,q.q1,q.q2,q.q3)
    R=QuaternionToRotation(q.q0,q.q1,q.q2,q.q3)
end

fclose(serial_handle)

%% Magic
ins=instrfind
fclose(ins)
delete(ins)
clear all
close all
clc