a(16) = -4; a(15)=1;a(3)=1;a(2)=-4;a(1)=6;
b = linspace(-1/15,-1/15,16);
b(1)=1;
b=1/16*b;
N=length(a);
clear y; y(N)=0; 
y(1)= 1;
%end of customizable code

Fa = fm(N,-1)*a';%multiplies a by the 16x16 inverse forier matrix
Fb = fm(N,-1)*b';%multiplies b by the 16x16 inverse forier matrix
Fy = Fb./Fa*1/N;
Fy(1)=0;%Assigns 0 temporarily to Fy(1) for calculations below

ones = linspace(1,1,N);
%let Fy(1) = x such that y(1) = 1
%F^(-1)*Fy = y
%the first row of F^(-1) is composed of 1/N
%[1 1 1 ...]*[x Fy(2) Fy(3) .... ] * 1/N = y(1)
Fy(1) = y(1)-ones*Fy;

y = fm(16,1)*Fy;

AllImaginary =1;
AllReal=1;
tolerance = 10^3;
for n=1:N
    im =imag(y(n));
    rea = real(y(n));
    if (abs(im*tolerance) > abs(rea))
        disp(im);
        disp(rea);
        AllReal=0;
        continue; 
    end
    if abs(real(y(n))*tolerance) >  abs(imag(y(n)))
        AllImaginary =0;
        continue; 
    end
AllImaginary =0;
AllReal=0;
end

if AllImaginary==1
   y = imag(y);    
end
if AllReal==1
   y = real(y);    
end
clear('tolerance','AllImaginary','AllReal');

temp=abs(convolute(a,y)-b);
for n=1:N
    if temp(n) > 10^(-11)
        disp('possible error!');
    end
end
clear('temp','n','errors','rea','im')

plot(0:N-1,y,'*r', 'markersize', 10); hold on;
plot(0:N-1,y,'-b');
xlabel('Component index k');
ylabel('Solution Vector');
title('Solution of a convolution equation');
