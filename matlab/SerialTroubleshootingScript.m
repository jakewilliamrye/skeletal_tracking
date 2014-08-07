%% SerialTroubleshootingScript

%% Show connected instruments and status
ins=instrfind

%% Close all instruments
fclose(ins)
ins=instrfind

%% Remove all known instruments
delete(ins)

%% Clear matlab environment
close all
clear all
clc