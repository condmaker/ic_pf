## Cillionaire - Um jogo de 'Quem quer ser milionário' feito em C
### Made by Marco Domingos, 21901309
### Team: **Nameless Jeff**

Este relatório pretende descrever a solução do problema e exemplificar a estrutura do software *cillionaire.c*, onde o código do jogo se encontra.

---
#### Descrição da Solução
Como objeto de trabalho final, o programa teve vários pormenores para serem feitos e pensados. Veremos eles um por um.

* Argumentos principais

O programa em sua inicialização recebe dois argumentos, *-s* e *-f*, o *-s* recebe uma seed e usa a função *atoi()* para convertê-la em *int* e o *-f* recebe o nome de um ficheiro, que é aberto e enviado para a função *question_read()*, que irá ler o ficheiro, e depois retornar, fechando-o assim.

O *argc* irá verificar se há menos de um argumento e aplicar *srand(time)* caso isto aconteça. se forem dados mais de 5 argumentos o programa disponibiliza um erro. Ele verifica também que se mais de 3 argumentos forem dados, ele irá ver se argv[1] == argv[3] (isto irá verificar se o jogador colocou o comando -s e -f duas vezes) e caso isto não seja o caso, ele continua, verificando se o próximo é *-s* ou *-f* dependendo no primeiro argumento dado, aceitando então o input de *-f* e *-s* não importando a posição dos dois.

* Menu principal e o loop de opções

O menu principal, que contém todas as funções importantes, foi feito com um símples loop *while* que sempre rodará, há não ser que o jogador ofereça o input 'q', que está dentro de um dos *ifs*, dando *break* no loop. O jogo lê o input do jogador com um *fgets()*.

A chamada *n* irá ver se o segundo caractere do input é diferente de '\n', e caso for, irá interpretar aquilo como um nome, utilizando um for para igualar cada caractere do input do jogador após o n e um espaço para currentUser.playername, a estrutura de dados do jogador atual. Depois disto ele adiciona um '\0' ao último index fora do *for* para acabar a string e iguala o index à 0. Caso o jogador apenas coloque n e o segundo caractere seja igual a '\n', o jogo automaticamente grava o nome do jogador como 'Newbie'. O jogo após verificar todos estes protocolos de nome iguala os dois jokers do jogador à 1 e entra na função *new_game()*

As outras chamadas, *c* e *h* são símples, e apenas chamam funções para fazerem o que precisam.

As chamadas *s* e *r* respetivamente salvam e leem ficheiros de jogo.
A *s* cria um ficheiro novo em modo binário, e grava em linhas diferentes e seguidas os dados do jogador em binário. É dado o ponteiro de playerName (já que é uma string) e os endereços de memória do nível e dos dois jokers, fechando depois a file e saindo do programa. Caso o ficheiro aberto esteja vazio ele irá sair do programa com um erro.
A *r* abre um ficheiro e lê linha por linha em binário o que foi gravado num ficheiro. Caso o ficheiro aberto esteja vazio ele irá sair do programa com um erro.

* Lista ligada de perguntas

Na função question_read(), ele irá inicializar um ponteiro char para guardar as linhas do ficheiro aberto, e irá abrir espaço no heap para este. Duas variáveis de ponteiro QUESTION serão abertas e colocadas no heap, e dadas *NULL* no inicio. Estas serão HEAD e TAIL. HEAD irá apontar para o topo da lista, enquanto TAIL irá percorrer o resto da lista, a criando. Também será criada uma váriavel question new, que irá guardar as informações da lista ligada.

A estrutura de dados QUESTION, inicializada no inicio do programa, contém uma string *char question* que irá salvar a pergunta da linha, uma matriz *char answers* que contém as respostas nas respetivas posições, um tipo enumerado com as dificuldades, e um nó para a próxima parte da estrutura.

Ele irá usar um loop *while* com um *fgets()* que irá loopar até que este retorne NULL, ou seja, já não existam mais linhas. Se o primeiro caractere da linha lido for ';' este dá *continue* para ler a próxima linha. Caso contário, ele irá assumir que este inicia-se com QUESTION, e irá pular caractéres para cada linha (assumindo que a órdem de linhas é QUESTION - OPTION0 - OPTION1 - OPTION2 - OPTION3 - CATEGORY - DIFFICULTY), e ler tudo, passando a linha salva em question_vect para cada parte da estrutura respetivamente. É utilizado um *strcmp()* para verificar a dificuldade. Após isto ser feito, ele vai verificar se HEAD é igual a NULL (só o ocorre na primeira instância) e irá conectar HEAD e TAIL com new. Caso contrário, o atributo next de TAIL será igual a new (já que TAIL será a lista anterior àquela que está sendo lida), apontando para a nova lista sendo lida, e o next de new será NULL, já que ainda não
estará apontando para nada (e se for a última a ser lida, irá apontar para NULL), após isto, TAIL é igualado a new para a lista continuar.

Quando o loop termina, TAIL e question_vect são liberados com *free()* do heap, já que não serão mais utilizados. HEAD, que aponta para o topo da função, e new, que contêm a lista, serão devolvidos numa struct.

* Informações do jogador

São guardadas numa estrutura de dados PLAYERINFO que contém nome, nível e a existência de jokers 25:75 ou 50:50.

* Jogabilidade

O jogo, que inicia na função *new_game()* irá criar várias variáveis iniciais, como uma string *char optionVect* que irá conter as letras ABCD de resposta, uma váriavel de ponteiro QUESTION que irá conter a lista ligada de acordo com HEAD, e um *int r* que será um número randómico de 0 a 4 (apanha um número random e o resto de sua divisão inteira por 4) e irá, com uma secção de *ifs*, decidir a posição das respostas baseado neste número.

O programa contém um loop *for* que irá igualar *aux* à question_read.head e ir a um elemento novo da lista toda vez que este faz o loop em sí, até aux ser igual à NULL.

No inicio do programa ele verifica se o nível anterior do jogador era de dificuldade diferente, e caso seja, este irá igualar aux à question_read.head novamente para retornar ao topo da lista. Após isto, ele irá verificar se o nível do jogador e sua dificuldade correspondem ao nível de dificuldade da pergunta, e caso estes não sejam equivalentes, ele irá dar um *continue* e retornar ao início do loop.

Quando o jogador digita a resposta ele, dependendo do *r*, irá verificar se o input do jogador é igual ao caractére correto, e caso for, irá aumentar um *int* já declarado lvliter por um, e irá igualar o nível do jogador a levels[lvliter], que é uma string já declarada com todos os níveis de jogo. Outra *int*, erriter, que verifica o número de erros do jogador e só vai até 2 até que o jogo acabe por excesso de erros, será retornada a 0 caso seja igual a 1 neste caso.

Se o nível do jogador for igual a levels[8], o último nível, o jogo irá imprimir mensagens de congratulação e fechar o programa.

Se o jogador errar, este irá iterar o erriter em 1, e verificar se este é igual a 2 (se sim, o jogo irá terminar). Após isto ele irá verificar se o lvliter é diferente de 0 (se lvliter for igual a 0 e este for retirado 1, lvliter será igual a -1, causando problemas no código visto que só é possível termos números positivos nos níveis.) e retirar -1 se for. Após isto o programa irá igualar currentUser->level à levels[lvliter].

Toda vez que o jogador responde à pergunta, o jogo irá recalcular *r*.

O jogador também pode chamar os comandos *c*, *h*, *q* e *s*, também como os comandos *j 50* e *j 25*.

Os comandos *j 50* e *j 25* verificam se o jogador, com *currentUser.j50* ou *currentUser.j25* têm respetivos jokers, e caso não tenham irá retornar a pergunta. Caso tenham, estes serão retirados no fim do código do comando. Uma outra variável, *int jkrturn*, será ativada quando o jogador utiliza o joker numa partida, e quando ativada o jogo não deixa que o usuário utilize mais jokers naquela ronda. Apenas após este digitar a resposta que jkrturn será desativado. Os comandos de joker utilizam a mesma lógica das perguntas-- utilizando o *r* para decidir o espaço onde são colocadas. 

---
### Estrutura do Software
Esta secção irá descrever as funções do software e o que estas fazem.

* Funções de comando
    - Função *n*

      Pode apenas ser utilizada no ecrã inicial. O utilizador dá um nome a frente do comando e o jogo grava o nome do jogador. Caso nenhum argumento seja dado, o nome será automaticamente dado como *"Newbie"*.

    - Função *s*
  
      Salva o estado do jogador (seu nome, nível, e quantos jokers este têm), e pode ser utilizado em qualquer ponto do jogo. Após o comando ser utilizado, o jogo irá salvar o progresso num ficheiro e fechar.

    - Função *r*

      Lê o ficheiro que o jogador escreve após a letra *r* no *input*. Caso este não consiga abrir, irá dar uma mensagem de erro e fechar o programa. No contrário, será lido o ficheiro do jogador e as suas informações de jogatina (nome, nível e jokers) serão assimiladas a sessão atual do programa. Apenas pode ser utilizada no início do programa.

    - Função *c*

      Imprime os créditos do jogo em qualquer ponto do programa.

    - Função *h*

      Imprime o menu principal novamente em qualquer ponto do programa.

    - Função *j*

      O primeiro argumento dos *jokers*, que retiram respostas erradas das perguntas. Apenas um joker pode ser usado por pergunta.

      - *j 50*

        Utiliza um *joker 50:50* do jogador, retirando duas respostas erradas da pergunta. Apenas pode ser utilizada no jogo e uma vez por jogador.

      - *j 25*

        Utiliza um *joker 25:75* do jogador, retirando uma resposta errada da pergunta. Apenas pode ser utilizada no jogo e uma vez por jogador.  

    - Função *q*
      Pode ser utilizada a qualquer momento, e irá sair do jogo.

* Argumentos principais
    - Argumento *s <int>*
        
      Irá definir a *seed* do programa. É necessário dar um número a frente para que o programa não dê erro.

    - Argumento *f <filename>*
  
      Irá definir o nome do ficheiro de perguntar que o programa irá procurar na pasta e ler para criar a lista ligada de perguntas. 

    Ambos os argumentos podem ser dados na linha de comando, não importando sua órdem.

* Função *new_game()* 
    Irá apresentar aos jogadores as perguntas da lista em relação ao nível e a dificuldade do jogador. Quando o jogador acerta uma questão, este irá subir um nível, e em semelhança, quando erra, irá descer um nível. Caso o jogador erre duas questões de seguida, este irá perder o jogo e o mesmo irá fechar.

    O jogo têm 8 níveis, e quando o jogador passa o oitavo nível, este acaba. Os níveis estão dividos em patamares, como é visto aqui em órdem crescente:

    > 0, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000

    Os níveis 0 à 1000 têm dificuldade *easy* nas perguntas,
    Enquanto os níveis 2000 e 5000 têm dificuldade *medium* nas perguntas,
    E finalmente, os níveis 10000 à 100000 têm dificuldade *hard* nas perguntas.

---
### Conclusões e matéria aprendida
A produção deste trabalho foi complicada, e retirou bastante de meu tempo. Porém aprendi matéria substancial aqui, pela prática. Acho que a utilização de listas ligadas e a leitura de ficheiros foram os dois tópicos que me deram mais trabalho, porém agora que estão implementados e os percebo, penso que posso fazer mais muita coisa com ficheiros de fora e estruturas de dados. Meu conhecimento de ponteiros, que pensava que estava bom antes, foi melhorado mais ainda, com a vasta utilização destes na implementação de listas ligadas.

A complexidade do programa relativa aos outros programas já feitos por mim também é alta, que ajudou-me a pensar mais lógicamente num projeto de escala um bocado maior como este.

---
### Referências
Aqui estarão as referências utilizadas para criação do programa.

A implementação da lista ligada em formato FIFO (first in first out), junto com o TAIL, foi pensada junto com o aluno da Universidade Lusófona Daniel Fernandes. E a criação do algorítmo para implentação do número random de 0 à 4 para definir a posição das respostas foi do grupo *#DreamTeam*, neste mesmo trabalho final para a Universidade Lusófona.
