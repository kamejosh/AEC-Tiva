% clear command window
clc;

% clear workspace
clear all;
% close open windows
close all;

%mex c file
mex aec.c

%create a one second sinus at 440Hz
t = 0:(1/8000):20;
sinus = sin(2*pi*440*t);
farend = randn(size(sinus));

nearend = sinus + farend*0.3;

tic;
cleaned = aec(nearend, farend, sinus);
timeFor1Second = toc;

disp(timeFor1Second);

% sound(nearend);
% pause(1)
% sound(cleaned);

figure
% plot near-end signal
subplot(3,1,1);
plot(nearend);
title('nearspeech');
% plot echoed signal
subplot (3,1,2);
plot (cleaned);
title('cleaned');

%% Plotting erle

leftOver = nearend - cleaned;
erle = leftOver ./ farend;

subplot (3,1,3);
plot(erle);
title('erle');
