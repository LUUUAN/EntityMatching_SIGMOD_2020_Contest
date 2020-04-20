def calculate_Edit_Distance(str1, str2):
    str1 = str1.lower()
    str2 = str2.lower()
    n1 = len(str1)
    n2 = len(str2)
    n = max(n1, n2) + 7
    dp = [[0 for i in range(n)] for j in range(n)]
    INF = 0X3F3F3F3F
    for i in range(1, n1 + 1):
        for j in range(1, n2 + 1):
            dp[i][j] = INF
    for i in range(1, n1 + 1):
        dp[i][0] = i
    for j in range(1, n2 + 1):
        dp[0][j] = j
    for i in range(1, n1 + 1):
        for j in range(1, n2 + 1):
            flag = 1
            if str1[i - 1] == str2[j - 1]:
                flag = 0
            dp[i][j] = min(dp[i - 1][j] + 1, min(dp[i][j - 1] + 1, dp[i - 1][j - 1] + flag))
    return dp[n1][n2]