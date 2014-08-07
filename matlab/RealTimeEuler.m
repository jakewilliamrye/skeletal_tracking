%% MainImuScript

%% Setup
% Serial Comminication
serial_handle=serial('COM73', 'BaudRate', 9600,'InputBufferSize',1024,'ReadAsyncMode','continuous');
fopen(serial_handle)

% Data Structures
q=Quaternion;
[psi,theta,phi]=QuaternionToEuler(q.q0,q.q1,q.q2,q.q3)
R=QuaternionToRotation(q.q0,q.q1,q.q2,q.q3);
 
% Phi Plot Handle
figure_handle_phi = figure();
axes_handle_phi = axes('Parent',figure_handle_phi,'XGrid','on','YGrid','on');
plot_handle_phi=plot(axes_handle_phi,[0,cos(phi)],[0,sin(phi)])
axis([-1,1,-1,1]); title('Phi');

% Theta Plot Handle
figure_handle_theta = figure();
axes_handle_theta = axes('Parent',figure_handle_theta,'XGrid','on','YGrid','on');
plot_handle_theta=plot(axes_handle_theta,[0,cos(phi)],[0,sin(phi)])
axis([-1,1,-1,1]); title('Theta');

% Psi Plot Handle
figure_handle_psi = figure();
axes_handle_psi = axes('Parent',figure_handle_psi,'XGrid','on','YGrid','on');
plot_handle_psi=plot(axes_handle_psi,[0,cos(psi)],[0,sin(psi)])
axis([-1,1,-1,1]); title('Psi');

%% Stream Data
while(1)
    q=SerialReceive(serial_handle)
     [psi,theta,phi]=QuaternionToEuler(q.q0,q.q1,q.q2,q.q3)
     R=QuaternionToRotation(q.q0,q.q1,q.q2,q.q3);
     set(plot_handle_phi,'XData', [0 cos(phi)], 'YData', [0 sin(phi)]);
     set(plot_handle_theta,'XData', [0 cos(theta)], 'YData', [0 sin(theta)]);
     set(plot_handle_theta,'XData', [0 cos(psi)], 'YData', [0 sin(psi)]);
    
    pause(.1)
end
%% Magic
ins=instrfind
fclose(ins)
delete(ins)
clear all
close all
clc