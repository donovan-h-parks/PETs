
fout = open('arPETs9595.trees.subsampled.txt', 'w')
for line in open('arPETs9595.trees.txt'):
	if line[0] == '%':
		fout.write(line)
		count = 0
	elif count < 10:
		fout.write(line)
		count += 1
fout.close()
		