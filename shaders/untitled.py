	gl_Position = pers * base * (x * y * z) * vec4(aPos, 1.0f);
	gl_Position = pers * base * (x * z * y) * vec4(aPos, 1.0f);
	gl_Position = pers * base * (y * x * z) * vec4(aPos, 1.0f);
	gl_Position = pers * base * (y * z * x) * vec4(aPos, 1.0f);
	gl_Position = pers * base * (z * x * y) * vec4(aPos, 1.0f);
