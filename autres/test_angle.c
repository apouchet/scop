

mx->rotate_x[0][0] = 0;
mx->rotate_x[0][1] = cosf(angleX) + sinf(angleX);
mx->rotate_x[0][2] = cosf(angleX) - sinf(angleX);

mx->rotate_y[1][0] = cosf(angleY) + sinf(angleY);
mx->rotate_y[1][1] = 0;
mx->rotate_y[1][2] = cosf(angleY) - sinf(angleY);


mx->rotate_z[2][0] = cosf(angleZ) + sinf(angleZ);
mx->rotate_z[2][1] = cosf(angleZ) - sinf(angleY);
mx->rotate_z[2][2] = 0;


X * sinf(angleY);
X * cosf(angleY)
X * cosf(angleZ)
X * sinf(angleZ);

Y * cosf(angleX)
Y * sinf(angleX);
Y * cosf(angleZ)
Y * -sinf(angleZ);

Z * -sinf(angleX);
Z * cosf(angleX)
Z * cosf(angleY)
Z * -sinf(angleY);



ax = X;
ay = Y;
az = Z;
Y = Y * cosf(angleX) - az * sinf(angleX);
Z = Z * cosf(angleX) + ay * sinf(angleX);
ay = Y;
az = Z;
X = X * cosf(angleY) - az * sinf(angleY);
Z = Z * cosf(angleY) + ax * sinf(angleY);
ax = X;
X = X * cosf(angleZ) - ay * sinf(angleZ);
Y = Y * cosf(angleZ) + ax * sinf(angleZ);