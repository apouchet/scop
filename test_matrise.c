










// text[Y + 8 * i] = text[Y + 8 * i] * cosf(angleX) - text[Z + 8 * i] * sinf(angleX);
// text[Z + 8 * i] = text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX);
// text[Y + 8 * i] = tmp;

// text[X + 8 * i] = text[X + 8 * i] *  cosf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * sinf(angleY);
text[Z + 8 * i] = text[X + 8 * i] * -sinf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * cosf(angleY);
// text[X + 8 * i] = tmp;
tmpZ
tmpX
tmpZ = text[X + 8 * i] * -sinf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * cosf(angleY);
tmpX = (text[X + 8 * i] *  cosf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * sinf(angleY)) * cosf(angleZ) - (text[Y + 8 * i] * cosf(angleX) - text[Z + 8 * i] * sinf(angleX)) * sinf(angleZ);
text[Y + 8 * i] = (text[X + 8 * i] *  cosf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * sinf(angleY)) * sinf(angleZ) + (text[Y + 8 * i] * cosf(angleX) - text[Z + 8 * i] * sinf(angleX)) * cosf(angleZ);
text[X + 8 * i] = tmpx;
text[Z + 8 * i] = tmpZ;






tmpZ = text[X + 8 * i] * -sinf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * cosf(angleY);

tmpX = (text[X + 8 * i] *  cosf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * sinf(angleY)) * cosf(angleZ)
- text[Y + 8 * i] * cosf(angleX) * sinf(angleZ) - text[Z + 8 * i] * sinf(angleX) * sinf(angleZ);

text[Y + 8 * i] = (text[X + 8 * i] *  cosf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * sinf(angleY)) * sinf(angleZ)
+ (text[Y + 8 * i] * cosf(angleX) - text[Z + 8 * i] * sinf(angleX)) * cosf(angleZ);

text[X + 8 * i] = tmpX;
text[Z + 8 * i] = tmpZ;