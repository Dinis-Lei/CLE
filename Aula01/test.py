with open("dataSet1/text0.txt", 'r') as file:

    words = 0
    vowel_count = [0]*5
    for line in file:
        line = line.split(" ")
        #print(line)
        for word in line:
            words += 1
            word = word.lower()
            if 'a' in word:
                vowel_count[0] += 1
            if 'e' in word:
                vowel_count[1] += 1
            if 'i' in word:
                vowel_count[2] += 1
            if 'o' in word:
                vowel_count[3] += 1
            if 'u' in word:
                vowel_count[4] += 1
                #print(word)

    print(f"Number of words = {words}")
    print(f"Number of words with a = {vowel_count[0]}")
    print(f"Number of words with e = {vowel_count[1]}")
    print(f"Number of words with i = {vowel_count[2]}")
    print(f"Number of words with o = {vowel_count[3]}")
    print(f"Number of words with u = {vowel_count[4]}")
