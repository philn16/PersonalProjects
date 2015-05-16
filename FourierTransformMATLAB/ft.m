%f -> frequency, A -> Amplitude, k -> phase shift for cos(x+k)
function [f,A,k ] = ft(z,T)
N = length(z);
A(N)=0;
k(N)=0;
for n=0:N-1
%a and b are the coeficiants to
%A*cos(x-phase)= A*cos(phase)*cos(x)+A*sin(phase)*cos(x) written as
%A*cos(x-phase) = a*cos(x)+b*sin(x)
a=0;b=0;
for d=0:N-1
temp = 2*pi/N*n*d;
a = a +cos(temp)*z(d+1);
b = b +sin(temp)*z(d+1);
end
k(n+1) = atan(b/a);
if a==0 %prevents NAN 
k = sign(b)*pi/2; %if a==0 and b is something, the phase will be pi/2. Otherwise, it does not matter as A will be 0    
end
%cos(x-k)/(cos(k)*a)) = a*cos(n*x)+b*sin(n*x)
%A(n+1) = a*(1+(b/a)^2)^(1/2);
A(n+1) = sign(a)*(a^2+b^2)^(1/2); %simplification of A=a/cos(k);
end
clear('a','b','d','n','temp'); %temp variables used in for loop

f = [0:N-1].*1/T*(N-1)/N;
%Makes it so that oly positive ampliudes are returned
%and that -pi<phase shift < pi
for i=1:N
if A(i) < 0
A(i) = -A(i);
k(i) = k(i)+pi;
if k(i) > pi
k(i)=k(i)-2*pi;
end
if k(i) < -pi
k(i) = k(i)+2*pi;
end
end
end

