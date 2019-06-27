	cos[X] = cosf(angleX);
	sin[X] = sinf(angleX);
	cos[Y] = cosf(angleY);
	sin[Y] = sinf(angleY);
	cos[Z] = cosf(angleZ);
	sin[Z] = sinf(angleZ);
	mx->rotate[0][0] = cos[Y] * cos[Z];
	mx->rotate[1][0] = sin[X] * sin[Y] * cos[Z] + cos[X] * sin[Z];
	mx->rotate[2][0] = -cos[X] * sin[Y] * cos[Z] + sin[X] * sin[Z];
	// mx->rotate[3][0] = 0;

	mx->rotate[0][1] = - cos[Y] * sin[Z];
	mx->rotate[1][1] = - sin[X] * sin[Y] * sin[Z] + cos[X] * cos[Z];
	mx->rotate[2][1] = cos[X] * sin[Y] * sin[Z] + sin[X] * cos[Z];
	// mx->rotate[3][1] = 0;

	mx->rotate[0][2] = sin[Y];
	mx->rotate[1][2] = - sin[X] * cos[Y];
	mx->rotate[2][2] = cos[X] * cos[Y];
	// mx->rotate[3][2] = 0;

	// mx->rotate[0][3] = 0;
	// mx->rotate[1][3] = 0;
	// mx->rotate[2][3] = 0;
	mx->rotate[3][3] = 1.0f;