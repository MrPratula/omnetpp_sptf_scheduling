

with open('log.txt', 'r') as f:

    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T = ([] for i in range(20))

    for line in f:
        data = line.split("-")

        name = data[0]
        queue = data[1]
        length = data[2]

        if 0 < float(length) < 0.1:
            A.append(line)
        elif 0.1 <= float(length) < 0.2:
            B.append(line)
        elif 0.2 <= float(length) < 0.3:
            C.append(line)
        elif 0.3 <= float(length) < 0.4:
            D.append(line)
        elif 0.4 <= float(length) < 0.5:
            E.append(line)
        elif 0.5 <= float(length) < 0.6:
            F.append(line)
        elif 0.6 <= float(length) < 0.7:
            G.append(line)
        elif 0.7 <= float(length) < 0.8:
            H.append(line)
        elif 0.8 <= float(length) < 0.9:
            I.append(line)
        elif 0.9 <= float(length) < 1:
            J.append(line)
        elif 1 <= float(length) < 1.1:
            K.append(line)
        elif 1.1 <= float(length) < 1.2:
            L.append(line)
        elif 1.2 <= float(length) < 1.3:
            M.append(line)
        elif 1.3 <= float(length) < 1.4:
            N.append(line)
        elif 1.4 <= float(length) < 1.5:
            O.append(line)
        elif 1.5 <= float(length) < 1.6:
            P.append(line)
        elif 1.6 <= float(length) < 1.7:
            Q.append(line)
        elif 1.7 <= float(length) < 1.8:
            R.append(line)
        elif 1.8 <= float(length) < 1.9:
            S.append(line)
        else:
            T.append(line)


print(len(A))



