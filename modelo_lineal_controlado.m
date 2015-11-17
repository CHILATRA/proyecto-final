%------parametros fisicos----------%
L=0.3;
M=2;
m=0.1;
g=9.8;
ym=M/m;

%----------paramtros control---------------%
ts=1;
Mp=0.1;
A=(log(Mp))^2;
cita=sqrt(A/(pi^2+A));
Wn=4.6/(ts*cita);
PdI=-(cita*Wn)+(Wn*sqrt(cita^2-1))
PdII=-(cita*Wn)-(Wn*sqrt(cita^2-1))

%---espacio de estados-------funcion de transferencia------% 
A=[0 1 0 0; ...
   0 0 -g/ym 0; ...
   0 0 0 1; ...
   0 0 (g*(1+ym))/(L*ym) 0];
B=[0; 1/(m*ym); 0; -1/(m*L*ym)];
C=[1 0 1 0];
D=0;
estados=ss(A,B,C,D);
[FTN FTD]=ss2tf(A,B,C,D);

%--------calculo de K------------------------------%
Pd=[PdI PdII -5*cita*Wn -5*cita*Wn]
K=acker(A,B,Pd)
