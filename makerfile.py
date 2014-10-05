def maker(input1, output):
	notes = []
	length = []
	with open(input1) as inf:
		for line in inf:
			alpha = line.split()
			notes.append(alpha[0])
			length.append(alpha[1])
	inf.close()

	open(output, 'w').close()
	with open(output, 'w') as final:
		final.write('{\n')
		a = 0
		while a < len(notes):
			final.write('<')
			final.write(notes[a])
			final.write('>')
			final.write(length[a])
			final.write(' ')
			a+=1
		final.write('\n}')
	final.close()



maker('weee.txt', 'wee.ly')


