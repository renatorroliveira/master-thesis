
% identificacao
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{ufla}[2013/01/03]

% Compilacao preferencial em pdfLaTeX
\ifx\pdfoutput\undefined
\typeout{}
\typeout{*******************************************************************}
\typeout{* AVISO: Essa classe n�o foi feita para ser usada com LaTeX puro! }
\typeout{* Use pdfLaTeX!                                                   }
\typeout{*                                                                 }
\typeout{* Se voc� antes utilizava o comando                               }
\typeout{* latex arquivo; dvips arquivo                                    }
\typeout{* Ent�o basta agora digitar                                       }
\typeout{* pdflatex arquivo                                                }
\typeout{*                                                                 }
\typeout{* Dicas:                                                          }
\typeout{* - Ao usar imagens vetoriais, prefira exportar para pdf direto   }
\typeout{*   ou ent�o exporte para eps e utilize o comando                 }
\typeout{*   epstopdf arquivo.eps                                          }
\typeout{*   para gerar um pdf a partir de um eps                          }
\typeout{* - Ao usar imagens bitmaps, prefira o formato PNG.               }
\typeout{*******************************************************************}
\typeout{}
\stop
\fi

\LoadClass[11pt,letterpaper,oneside]{article}
\RequirePackage[T1]{fontenc}
\RequirePackage{ifthen}
\RequirePackage{calc}
\RequirePackage{lastpage}
\RequirePackage{graphicx}
\RequirePackage{setspace}
\RequirePackage{indentfirst}
\RequirePackage{pdfpages}
\RequirePackage{enumitem}
\RequirePackage{pslatex}

% Definção de margens
\setlength{\hoffset}{-1in} % remove padrão do LaTeX 
\setlength{\voffset}{-1in} % remove padrão do LaTeX 
\setlength{\oddsidemargin}{4.5cm}  % margem esquerda
\setlength{\evensidemargin}{4.5cm} % margem direita
\setlength{\topmargin}{1.2cm}    % margem superior
\newlength{\bottommargin}        % margem inferior é deduzida em LaTeX
\setlength{\bottommargin}{4.5cm} % margem inferior
\setlength{\headheight}{2.8cm} % altura do cabeçalho
\setlength{\headsep}{0.5cm}    % espaço entre cabeçalho e texto
\setlength{\footskip}{0cm}  % espaço entre rodapé e texto
\setlength\textheight{ % altura disponí­vel para texto
 \paperheight-\topmargin-\bottommargin-\headheight-\headsep-\footskip
} 
\setlength\textwidth{  % largura disponí­vel para texto
 \paperwidth-\oddsidemargin-\evensidemargin
}

\renewcommand{\baselinestretch}{1.5}\normalsize %espaçamento

% cabeçalho e rodapé
\RequirePackage{fancyhdr}
\fancyhead{}
\fancyhead[RO]{\nouppercase{\small\slshape\rightmark}}
\fancyhead[LE]{\nouppercase{\small\slshape\leftmark}}
\fancyfoot{}
\fancyfoot[CO,CE]{\small\thepage}

\fancypagestyle{plain}{
\fancyhf{}
%\fancyfoot[C]{\thepage}
\renewcommand{\headrulewidth}{0pt}
\renewcommand{\footrulewidth}{0pt}
}

\fancypagestyle{ufla}{
\fancyhf{}
\fancyhead[RO,RE]{\thepage}
\renewcommand{\headrulewidth}{0pt}
\renewcommand{\footrulewidth}{0pt}
}


% Tipos de fontes
\RequirePackage{pslatex} %times, helvetica e courier
\RequirePackage{times}
\RequirePackage{mathptmx} % fontes matemáticas em ps


% Controle de linhas orfãs e viúvas (desabilitar)
%\widowpenalty10000 % 10000 significa virtualmente infinito. 
%\clubpenalty10000


\newlanguage\nohyphen
\newcommand\nh[1]{\language\nohyphen #1}

% abstract e thanks
\renewcommand{\thanks}[1]{
                          \cleardoublepage
                          \null\vfill
                          \begin{center}\textbf{AGRADECIMENTOS}\end{center}
                          
							#1
                          
						  \vspace*{1cm}
                          \vfill\null
                          \clearpage
                         }

\def\palchaves#1{\gdef\@palchaves{\nh{#1}}} % palavras-chave da monografia
\palchaves{} %inicialmente vazio
\def\keywords#1{\gdef\@keywords{\nh{#1}}} % palavras-chave da monografia (em inglês)
\keywords{} %inicialmente vazio

\newcommand{\resumo}[1]{
\begin{center}\textbf{RESUMO}\end{center}
\begin{singlespace}

#1\vspace{22pt}\\
Palavras-chave: \@palchaves
\end{singlespace}
\newpage
}

\newcommand{\resumoingles}[1]{
\begin{center}\textbf{ABSTRACT}\end{center}
\begin{singlespace}

#1\vspace{22pt}\\
Keywords: \@keywords
\end{singlespace}
\newpage
}
\newcommand{\dedic}[1]{
                          \clearpage
                          \null\vfill
                          \begin{flushright}{\textit{#1}}\end{flushright}
                          \vspace*{1cm}
                          \clearpage
}
% código da monografia

\def\folhaaprovacao#1{\gdef\@folhaaprovacao{#1}}
\folhaaprovacao{\@empty}
\def\tipo#1{\gdef\@tipo{#1}} % tipo da monografia
\def\local#1{\gdef\@local{#1}} % local
\local{LAVRAS - MG}
\def\areaconcentracao#1{\gdef\@areaconcentracao{#1}} % área de concentracao
\def\orientador#1{\gdef\@orientador{#1}} % orientador
\def\coorientador#1{\gdef\@coorientador{#1}} % co-orientador
\coorientador{} %inicialmente vazio
\def\coorientadorbanca#1{\gdef\@coorientadorbanca{#1}} % co-orientador
\coorientadorbanca{}
\def\coorientadorbancainst#1{\gdef\@coorientadorbancainst{#1}} % co-orientador
\coorientadorbancainst{}
\def\coorientadordois#1{\gdef\@coorientadordois{#1}} % co-orientador pertence a banca?
\coorientadordois{} %inicialmente vazio}
\def\bancaum#1{\gdef\@bancaum{#1}} % 1. banca
\def\bancauminst#1{\gdef\@bancauminst{#1}}
\bancauminst{}
\def\bancadois#1{\gdef\@bancadois{#1}} % 2. banca
\def\bancadoisinst#1{\gdef\@bancadoisinst{#1}}
\bancadois{} % inicialmente vazia
\bancadoisinst{}
\def\bancatres#1{\gdef\@bancatres{#1}} % 2. banca
\def\bancatresinst#1{\gdef\@bancatresinst{#1}}
\bancatres{} % inicialmente vazia
\bancatresinst{}
\def\defesa#1{\gdef\@defesa{#1}} % data da defesa

% autor para ficha
\def\fcautor#1{\gdef\@fcautor{#1}}
\fcautor{}
\def\fcorientador#1{\gdef\@fcorientador{#1}}
\fcorientador{}
% tipo material
\def\fctipo#1{\gdef\@fctipo{#1}}
\fctipo{}
% catalogacao
\def\fccatalogacao#1{\gdef\@fccatalogacao{#1}}
\fccatalogacao{}
% classicacao 1
\def\fccdd#1{\gdef\@fccdd{#1}}
\fccdd{}


\newlength{\contentsnumberwidth}
\setlength{\contentsnumberwidth}{9ex}

%% dados da ficha catalográfica
% nome do curso
\def\curso#1{\gdef\@curso{#1}}

\newcommand{\tab}{\hspace*{0.7cm}}

%\newcommand{\entrylabel}[1]{\parbox[b]{\labelwidth}{\makebox[0pt][l]{\bfseries\uppercase{#1}}\\}}


% no iní­cio, redefinimos o comando de fazer tí­tulos
% e margens


\AtBeginDocument{
%=============
% capa
%=============
\begin{titlepage}
\pagestyle{empty}
\centering

% medidas com stretch são flexí­veis (redimensionam-se para ocupar
% o espaço que sobra da página

%\vspace*{2cm}
\includegraphics[width=0.35\textwidth]{ufla}
\vspace*{11pt}

\textbf{\large\MakeUppercase\@author} % autor da monografia

%\vspace{8cm}
\vspace{\stretch{8}}

\textbf{\Large\MakeUppercase\@title} % tí­tulo da monografia

%\vspace{3cm}
\vspace{\stretch{8}}

\textbf{\large\@local\\ 
\@date}

\end{titlepage}

\newpage % terminou capa

%=============
% folha de rosto
%=============

\begin{center}
\pagestyle{empty}
% Reseta Contagem de paginas
\pagenumbering{arabic}
\centering

%\vspace*{2cm}

\textbf{\MakeUppercase\@author} % autor da monografia

%\vspace{8cm}
\vspace{\stretch{3}}

\textbf{\MakeUppercase\@title} % tí­tulo da monografia

%\vspace{3cm}
\vspace{\stretch{2}}

\hfill\begin{minipage}{0.55\textwidth}
\begin{singlespace}
\@tipo
\end{singlespace}
\end{minipage} % tipo da monografia

%\vspace{3cm}
\vspace{\stretch{2}}

%Orientador Centralizado
Orientador\\
\@orientador

\ifthenelse{\equal{\@coorientador}{}}{}{
\vspace{\stretch{1}}
Coorientador\\
\@coorientador
}

\vspace{\stretch{3}}

\textbf{\@local\\ 
\@date}

\newpage % terminou folha de rosto

%=============
% ficha catalográfica
%=============
% Ainda ser� desenvolvida.


\null\vfill
  \renewcommand{\baselinestretch}{1.0}
  \centering
  \hfill \parbox{\textwidth}{
	\begin{singlespace}\begin{center}
		{ \normalfont\bfseries\normalsize Ficha Catalogr\'{a}fica Elaborada pela Coordenadoria de Produtos e Servi\c{c}os da Biblioteca Universit�ria da UFLA}
	\end{center}\end{singlespace}
  }

  \fbox{
    \parbox[r]{\textwidth}{
      \vspace*{11pt}
      \hfill \parbox[c]{\textwidth-6ex}{{\normalsize\normalfont
		 \begin{flushleft}
			\@fcautor \\
			\hspace*{3ex} \@title  /  \@author. -- Lavras : UFLA, \@date.\\
			\hspace*{3ex} 71 p. : il.\\ \vspace*{11pt}
			
			\hspace*{3ex} \@fctipo\\
			\hspace*{3ex} Orientador: \@fcorientador.\\
			\hspace*{3ex} Bibliografia.\\ \vspace*{11pt}
			
			\hspace*{3ex} \@fccatalogacao\\
		\end{flushleft}
		\begin{flushright}
			\@fccdd
		\end{flushright}
	  }}
    }
  }
\renewcommand{\baselinestretch}{1.5}
\newpage


%=============
% folha de aprovacao
%=============

\ifthenelse{\equal{\@folhaaprovacao}{\@empty}}{% PDF da folha nao definido.
\textbf{\MakeUppercase\@author}

\vspace{\stretch{1}}

\textbf{\MakeUppercase\@title}

\vspace{\stretch{1}}

\hfill\begin{minipage}{0.55\textwidth}
\begin{singlespace}
\@tipo
\end{singlespace}
\end{minipage}

\vspace{\stretch{1}}

\begin{flushleft}
APROVADA em \@defesa\hfill \\

\vspace*{15pt}

% primeira banca
\begin{tabular}{l@{\hskip 0.7cm}l}

\ifthenelse{\equal{\@coorientadorbanca}{\@empty}}{}{%
\@coorientadorbanca\hfill & \@coorientadorbancainst\hfill \\[11pt]
}
\@bancaum\hfill & \@bancauminst\hfill \\[11pt]

% segunda banca
\ifthenelse{\equal{\@bancadois}{\@empty}}{}   % se 2. banca nao existir, nao
                                                % produza nada
{
  \@bancadois\hfill & \@bancadoisinst\hfill \\[11pt]
}

\ifthenelse{\equal{\@bancatres}{\@empty}}{}   % se 3. banca nao existir, nao
                                                % produza nada
{
    \@bancatres\hfill & \@bancatresinst\hfill \\[11pt]
}
\end{tabular}
\end{flushleft}

\vspace{\stretch{1}}

\@orientador\\
Orientador

\ifthenelse{\equal{\@coorientador}{\@empty}}{}% se coorientador nao existir, nao produza nada
{\ifthenelse{\equal{\@coorientadorbanca}{\@empty}}{% S� se o coorientador nao for da banca.
    \vspace*{11pt}
    
    \@coorientador \\
    Coorientador
}{}}


\vspace{\stretch{1}} 

\textbf{\@local\\
\@date}

\newpage

}{% Pagina PDF definida.
\includepdf{\@folhaaprovacao}
}

\end{center}
}

% Define tabulacao do paragrafo.
\setlength{\parindent}{1.25cm}

\RequirePackage[titles]{tocloft}
\renewcommand{\cftsecfont}{\normalfont\normalsize\bfseries}
\renewcommand{\cftsubsecfont}{\normalfont\normalsize\bfseries}
\renewcommand{\cftsubsubsecfont}{\normalfont\normalsize\bfseries}
\renewcommand{\cftparafont}{\normalfont\normalsize\bfseries}
\renewcommand{\cftsubparafont}{\normalfont\normalsize\bfseries}

\renewcommand{\@dotsep}{1}
\newlength{\dotseplen}
\setlength{\dotseplen}{4pt}

\newlength{\defaultnumwidth}
\setlength{\defaultnumwidth}{\contentsnumberwidth}

\def\scalenumwidth#1{\gdef\@scalenumwidth{#1}}
\scalenumwidth{1}

\renewcommand{\l@section}{\normalfont\normalsize\bfseries\@dottedtocline{1}{0ex}{\@scalenumwidth\contentsnumberwidth}}
\renewcommand{\l@subsection}{\normalfont\normalsize\bfseries\@dottedtocline{2}{0ex}{\@scalenumwidth\contentsnumberwidth}}
\renewcommand{\l@subsubsection}{\normalfont\normalsize\bfseries\@dottedtocline{3}{0ex}{\@scalenumwidth\contentsnumberwidth}}
\renewcommand{\l@paragraph}{\normalfont\normalsize\bfseries\@dottedtocline{4}{0ex}{\@scalenumwidth\contentsnumberwidth}}
\renewcommand{\l@subparagraph}{\normalfont\normalsize\bfseries\@dottedtocline{5}{0ex}{\@scalenumwidth\contentsnumberwidth}}
%\renewcommand{\l@figure}{\@dottedtocline{1}{0ex}{10ex}}
%\renewcommand{\l@table}{\@dottedtocline{1}{0ex}{10ex}}

%\renewcommand{\numberline}[1]{\bfseries #1 }
%\def\figuresname{Lista de Figuras}
%\def\tablesname{Lista de Tabelas}

\setcounter{tocdepth}{5}
\setcounter{secnumdepth}{5}

\renewcommand{\tableofcontents}{
\begin{center}\textbf{\MakeUppercase{SUM�RIO}}\end{center}
\begin{singlespace}\normalfont\normalsize
 \@starttoc{toc}
\end{singlespace}
\newpage
}

\renewcommand{\listoffigures}{
\begin{center}\textbf{\MakeUppercase{\listfigurename}}\end{center}
\begin{singlespace}\normalfont\normalsize
 \@starttoc{lof}
\end{singlespace}
\newpage
}

\renewcommand{\listoftables}{
\begin{center}\textbf{\MakeUppercase{\listtablename}}\end{center}
\begin{singlespace}\normalfont\normalsize
 \@starttoc{lot}
\end{singlespace}
\newpage
}

\newcommand{\listofacronyms}[1]{
\begin{center}\textbf{\MakeUppercase{Lista de Siglas}}\end{center}
\begin{singlespace}\normalfont\normalsize
 \begin{itemize}[align=left,leftmargin=14ex,labelwidth=12ex,labelsep=2ex,itemsep=0pt,topsep=0ex,partopsep=0ex,parsep=0ex,font=\normalfont\normalsize]
	#1
 \end{itemize}
\end{singlespace}
\newpage
}

\RequirePackage{titletoc}
\titlecontents{figure}[0pt]{\filright}{\contentspush{\begin{minipage}{12ex}\figurename\ \thecontentslabel{}\ \end{minipage}}}{}{\titlerule*[\dotseplen]{.}\contentspage}
\titlecontents{table}[0pt]{\filright}{\contentspush{\begin{minipage}{12ex}\tablename\ \thecontentslabel{}\ \end{minipage}}}{}{\titlerule*[\dotseplen]{.}\contentspage}

\RequirePackage{caption}
\captionsetup{format=hang,justification=justified,labelsep=quad,singlelinecheck=false,font=small}

\RequirePackage{titlesec}
\titleformat{\section}[hang]{\normalfont\normalsize\bfseries}{\thetitle}{1em}{}
\titleformat{\subsection}[hang]{\normalfont\normalsize\bfseries}{\thetitle}{1em}{}
\titleformat{\subsubsection}[hang]{\normalfont\normalsize\bfseries}{\thetitle}{1em}{}
\titleformat{\paragraph}[hang]{\normalfont\normalsize\bfseries}{\thetitle}{1em}{}
\titleformat{\subparagraph}[hang]{\normalfont\normalsize\bfseries}{\thetitle}{1em}{}

% Comando para ser utilizado no lugar \bibliography.
\newcommand{\refstitle}[1]{
 \addcontentsline{toc}{section}{\hspace{\@scalenumwidth\contentsnumberwidth}#1}
 \newcommand{\dummysec}[1]{}
  \def\section*{\dummysec}
  \renewcommand{\refname}{}
  \begin{center}{\normalsize\bfseries #1}\end{center}
}%
\newcommand{\uflabibliography}[2]{
  \refstitle{#1}
  \renewcommand{\baselinestretch}{1}
  \bibliography{#2}
}%
\newcommand{\bibliografia}[1]{
	\uflabibliography{REFER\^{E}NCIAS}{#1}
}

\newcommand{\apendices}{
	\begin{center}
		{\normalfont\normalsize\bfseries AP\^{E}NDICE}
	\end{center}
	\addcontentsline{toc}{section}{\hspace{\@scalenumwidth\contentsnumberwidth}AP\^{E}NDICE}
	\newcounter{apendice}
	\renewcommand{\theapendice}{\Alph{apendice}}
}%

\newcommand{\apendice}[1]{
	\refstepcounter{apendice}
	\begin{center}
		{\normalfont\normalsize\bfseries AP\^{E}NDICE \theapendice{} - #1}
	\end{center}
}


\newcommand{\anexos}{
	\begin{center}
		{\normalfont\normalsize\bfseries ANEXOS}
	\end{center}
	\addcontentsline{toc}{section}{\hspace{\@scalenumwidth\contentsnumberwidth}ANEXOS}
	\newcounter{anexo}
	\renewcommand{\theanexo}{\Alph{anexo}}
}%

\newcommand{\anexo}[1]{
	\refstepcounter{anexo}
	\begin{center}
		{\normalfont\normalsize\bfseries ANEXO \theanexo{} - #1}
	\end{center}
}
