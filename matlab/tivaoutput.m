filename = 'outputfull.xlsx';
filename2 = 'subsinus.xlsx';
filename3 = 'noise.xlsx';

num = xlsread(filename);
num2 = xlsread(filename2);
num3 = xlsread(filename3);

num4 = num2 + num3;
% sound(num);

figure;
subplot (2,1,1);
plot(num4);
title('mic signal');
subplot (2,1,2);
plot(num);
title('output');