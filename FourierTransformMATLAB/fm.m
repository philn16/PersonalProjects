function [ fouriermatrix ] = fm( N,type )%-1 for inverse
%returns inverse only if type == -1
fouriermatrix(1)=0;%resets any previous variable
fouriermatrix(N,N)=0; %makes the matrix N by N
if (type ~=-1)%gives the normal forier matrix
for x=[0:N-1]%row
    for y=[0:N-1]%collum
        %w(x,y) = e^(2*pi*i/N*x*y) = cos(2*pi*x*y/N)+1i*sin(2*pi*x*y/N)
    fouriermatrix(x+1,y+1) = cos(2*pi*x*y/N)+1i*sin(2*pi*x*y/N);
        %   disp(fouriermatrix(x+1,y+1))
    end
end
else %gives the invers forier matrix
    %F^-1(x,y) = 1/N * w(-x*y) 
for x=[0:N-1]%row
    for y=[0:N-1]%collum
        %w(x,y) = e^(2*pi*i/N*i*j) = cos(2*pi*x*y/N)-1i*sin(2*pi*x*y/N)
    fouriermatrix(x+1,y+1) = cos(2*pi*x*y/N)-1i*sin(2*pi*x*y/N);
    end
end
fouriermatrix = fouriermatrix/N;
end
end

