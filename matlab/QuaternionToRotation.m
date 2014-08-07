function [R] = QuaternionToRotation(q0,q1,q2,q3)
    R=[2*q0*q0-1+2*q1*q1 2*(q1*q2+q0*q3) 2*(q1*q3-q0*q2);
       2*(q1*q2-q0*q3) 2*q0*q0-1+2*q2*q2 2*(q2*q3+q0*q1);
       2*(q1*q3+q0*q2) 2*(q2*q3-q0*q1) 2*q0*q0-1+2*q3*q3];
end