function [psi,theta,phi] = QuaternionToEuler(q0,q1,q2,q3)
    q0=double(q0); q1=double(q1); q2=double(q2); q3=double(q3); 
    psi=atan2(2*q1*q2-2*q0*q3, 2*q0*q0+2*q1*q1-1);
    theta=-asin(2*q1*q3+2*q0*q2);
    phi=atan2(2*q2*q3-2*q0*q1, 2*q0*q0+2*q3*q3-1);
end

