function f = plotQuiverBackupEnv(S)
% Do a quiver plot of the states in the truck backer-upper environment.
% Argument 'S' is an N x 4 matrix of the states where the dimensions
% are the x coordinate, the y coordinate, angle of semi, and angle
% of cab respectively.

[rows, cols] = size(S);

if(cols ~= 4)
     warning('S does not contain 4 states, returning');
     return;
end

% State vector.

x = S(:, 1);
y = S(:, 2);
ts = S(:, 3);
tc = S(:, 4);

% Get the vectors representing the semi.

xvs = 7 * cos(ts);
yvs = 7 * sin(ts);

quiver(x, y, xvs, yvs, 0, '.');

% Get the vectors representing the cab.

xvc = 3 * cos(tc);
yvc = 3 * sin(tc);

% Coordinates of cab.

xc = x + xvs;
yc = y + yvs;

hold on;
quiver(xc, yc, xvc, yvc, 0, 'r');
hold off;

% Concatenate vector matrices.
% 
% px = [x; xc];
% py = [y; yc];
% 
% pxv = [xvs; xvc];
% pyv = [yvs; yvc];
% 
% 
% quiver(px, py, pxv, pyv, 0, '-r');
%quiver(x, y, xvs, yvs);
