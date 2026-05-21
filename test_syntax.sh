echo 'echo 1 | | echo 2'
echo 1 | | echo 2

echo 'echo 2 || && echo 2'
echo 2 || && echo 2

echo 'echo 1 | || echo 2'
echo 1 | || echo 2

echo 'echo 1 || | echo 2'
echo 1 || | echo 2

echo 'echo 1 > | nofile'
echo 1 > | nofile

echo 'echo 1 > >nofile'
echo 1 > >nofile

echo 'echo &&  || echo 2'
echo &&  || echo 2

echo 'echo 1 ( echo 1)'
echo 1 ( echo 1)

echo 'echo > (echo 1)'
echo > (echo 1)

echo '(echo hola > ) > nofile'
(echo hola > ) > nofile

echo 'echo 1 )'
echo 1 )

echo 'echo 1 >'
echo 1 >
