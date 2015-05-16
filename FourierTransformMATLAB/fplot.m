function [ y ] = fplot( x,A,k,f )
clear('y');
N = length(A);
prec = length(x);
y(prec)=0;
y(prec)=0;
for c=1:prec %prec = graphing precision
y(c)=0; %just in case y somehow gains a non-zero value
for n=0:N-1
y(c)=y(c)+A(n+1)*cos(2*pi*f(n+1)*x(c)-k(n+1)); %sums up amplittudes * cos(f*x+phase_shif)
end
y(c) = y(c)/N;
end
end

