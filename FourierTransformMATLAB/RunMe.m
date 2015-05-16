%set the z vector to be anything that you desire, and it will be broken up into sines and cosines
z = [5.3566 -4.9402 1.5454 -1.5092 0.8289 2.5906 -5.2015 5.3881 -2.5281 -0.7167 -0.3738 1.5092 1.9995 -2.5906 -1.6269 0.2688];
%z = [1 1 1 -1 -1 -1 1 1 1 -1 -1 -1 1 1 1 -1 -1 -1]
T=0.05; %The end time when the signal ended. 
prec = 1500; %graphing precision; total number of points to be plotted. 
x = linspace(0,T,prec); %the x values to be ploted. Somewhat arbitrary; does not affect frequency
%end of customizing code
N=length(z);
%Next two lines are for finding the interpolated signal
[f,A,k] = ft(z,T); %sets frequency -> f, amplitude -> A, phase shift -> k
y = fplot(x,A,k,f); %the y values of the interpolated signal

Fz = fm(N,-1)*z';
FzReal = real(Fz);
FzImag = imag(Fz);

figure();
set(gcf, 'name', 'Plot of Z and interpolated signal')
 hold off
 plot(linspace(0,T,length(z)),z,'*r', 'markersize', 10);
 hold on
 plot(x,y,'b');
 xlabel('Time (seconds)');
 ylabel('Signal Strength');
 title('Interpolated signal and sampled points');
 hold off
%  
% figure();
% set(gcf, 'name', 'Real Constants F(z)')
% plot(0:N-1,FzReal,'-b');
% hold on
% plot(0:N-1,FzReal,'*r');
% xlabel('Component index');
% ylabel('Real Value');
% title('Real Constants F(z)');
% 
% figure();
% set(gcf, 'name', 'Imaginary Constants F(z)')
% plot(0:N-1,FzImag,'-b');
% hold on;
% plot(0:N-1,FzImag,'*r');
% xlabel('Component index');
% ylabel('Imaginary Value');
% title('Imaginary Constants F(z)');
