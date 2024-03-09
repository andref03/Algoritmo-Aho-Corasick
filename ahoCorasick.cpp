#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;
int totalAcessos = 0;

void printExecutionTime(chrono::steady_clock::time_point start, chrono::steady_clock::time_point end) {
    auto diff = end - start;
    cout << endl << "Tempo de execucao em segundos: "
         << chrono::duration<double, ratio<1>>(diff).count() << " s" << endl;
}

//estrutura dos nós da Árvore Trie
struct Node {
    unordered_map<char, Node*> filho;
    Node* falha;
    vector<int> saidas;

    //construtor que inicia todos os nós com falha = null
    Node() : falha(nullptr) {} 

};


//função que vai criar a Árvore Trie inserindo pattern
void arvoreTrie(Node* raiz, const string &pattern, int indiceAtual) {
    
  Node* nodeAtual = raiz;

    for (char ch : pattern) {
        if (nodeAtual->filho.find(ch) == nodeAtual->filho.end()) {
            //se não existe um nó com esse caractere ainda, cria-se esse nó
            nodeAtual->filho[ch] = new Node(); 
        }

        nodeAtual = nodeAtual->filho[ch]; //avança o nó
    }

    //o final de cada padrão é identificado pelo indiceAtual, para saber que determinado padrão foi encontrado
    nodeAtual->saidas.push_back(indiceAtual);
    //incrementa o contador de acessos
    totalAcessos ++;
}


//construindo a função de falha usando BFS (busca por largura)
void failFunction(Node* raiz) {
    
  queue<Node*> fila; //fila pra identificar os nós já lidos e não lidos

    for (auto &atual : raiz->filho) {
        //o campo falha, de cada filho da raiz, recebe o endereço do nó raiz
        //e a fila recebe esses filhos.
        atual.second->falha = raiz;
        fila.push(atual.second);
        totalAcessos++;
    }

    //identificando as falhas de cada nó da árvore
    while (!fila.empty()) {
      
        Node* nodeAtual = fila.front();
        fila.pop();

        for (auto &it : nodeAtual->filho) {
          
            char ch = it.first; //caractere que existe nesse filhoAtual
            Node* filhoAtual = it.second;

            //posteriormente, esse filhoAtual será analisado (é colocado na fila)
            fila.push(filhoAtual);

            Node* nodeFalha = nodeAtual->falha;
            //condição: o nodeFalha não pode ser nulo e ele não pode conter um filho de caractere ch
            while (nodeFalha != nullptr && nodeFalha->filho.find(ch) == nodeFalha->filho.end()) {
                nodeFalha = nodeFalha->falha; //retrocede pelo nó através da campo falha
            }

            filhoAtual->falha = 
              (nodeFalha == nullptr) ? raiz : nodeFalha->filho[ch]; //define a falha desse filhoAtual

            //se o filhoAtual é o fim de um padrão e esse padrão contém uma outra palavra padrão, devemos considerar os dois padrões 
            filhoAtual->saidas.insert(filhoAtual->saidas.end(), filhoAtual->falha->saidas.begin(), filhoAtual->falha->saidas.end());
          
        } //fim do laço for

        totalAcessos++;
      
    } //fim do laço while
}


//enocntrando os padrões em patterns simultaneamente no buffer
void ahoCorasick(Node* raiz, const deque<string> &texto, const vector<string> &patterns) {
    
    Node* nodeAtual = raiz;

    for (int i = 0; i < texto.size(); i++) {
      
        const string &linhaAtual = texto[i]; //armazena a linha atual do texto na string linhaAtual, para analisá-la

        for (int j = 0; j < linhaAtual.length(); j++) {
            
            char ch = linhaAtual[j]; //armazena o caractere atual da linha em ch, para analisá-lo

            //condição: o nodeAtual não pode ser nulo e ele não pode conter um filho de caractere ch
            while (nodeAtual != nullptr && nodeAtual->filho.find(ch) == nodeAtual->filho.end()) {
                nodeAtual = nodeAtual->falha; //retrocede pelo nó através da campo falha
            }

            if (nodeAtual == nullptr) {
                nodeAtual = raiz;
                continue;
            }

            //o nodeAtual agora avança pelo filho encontrado em comum com o caractere atual da linha
            nodeAtual = nodeAtual->filho[ch];

            for (int idx : nodeAtual->saidas) {
                  cout << "[Linha " << i+1 << "]\tEncontrado o padrao \"" << patterns[idx] << "\"" << endl;
                  /* << ", posicao " << j - patterns[idx].length() + 2  */
                  totalAcessos++;
            }
          
        } //fim do laço for da linha atual
      
    } //fim do laço for do texto
}

//--------- FUNÇÃO MAIN -----------------------------------------------------

int main() {

    //---- VARIÁVEIS E ESTRUTURAS ------------------

    chrono::steady_clock::time_point begin, end;
    Node* raiz = new Node();
    vector<string> patterns = {"inte", "comp", "tica","log"}; //padrões buscados

    //cria a Árvore Trie com os padrões acima
    for (int i = 0; i < patterns.size(); i++) {
        arvoreTrie(raiz, patterns[i], i);
    }

    //construindo a função de falha
    failFunction(raiz);

    //o arquivo só precisa ser lido uma única vez (busca simultânea)
    ifstream arquivo("entrada.txt"); 

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        exit(1);
    }

    totalAcessos ++;
  
    const size_t tamanhodoBuffer = 2048; //qtdd de caracteres do buffer que declaro abaixo
    deque<string> buffer;
    string linha;

    cout << endl << "ALGORITMO AHO-CORASICK" << endl << endl;

    //---- REALIZANDO LEITURA E BUSCA DOS PADRÕES ------------

    begin = chrono::steady_clock::now();
  
    while (getline(arquivo, linha)) {
        buffer.push_back(linha);

        if (buffer.size() > tamanhodoBuffer) {
            //busca os padrões dentro do buffer atualmente lido
            ahoCorasick(raiz, buffer, patterns);
            buffer.pop_front();
        }
      //incrementa o contador de acessos durante a leitura
      totalAcessos ++;
    }

    //provavelmente o texto não totalmente lido ainda, então busca os padrões no restante
    ahoCorasick(raiz, buffer, patterns);

    end = chrono::steady_clock::now();
  
    arquivo.close();
    delete raiz;

    //imprime o tempo de begin até o end (nesse caso, compreende todo o laço for)
    printExecutionTime(begin, end);

    //imprime o total de acessos
    cout << "Total de acessos: " << totalAcessos << endl;

    return 0;
}
