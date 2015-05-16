function [ cv ] = convolute( z,y )
N = length(z);
clear cv;
cv(N)=0;

for k=0:N-1
c1 = k;
    for j=0:N-1
    cv(k+1)=cv(k+1)+z(c1+1)*y(j+1);
    c1 = c1 -1;
        if c1 < 0
        c1 = N-1; 
        end
    end
end
end

