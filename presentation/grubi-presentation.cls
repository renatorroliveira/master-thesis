% Classe GRUBI Presentations - Apreseta��es do Grupo de Redes Ub�quas da UFLA
%
% Encoding do Arquivo: Western European ISO-8859-1 (latin1)
% Criada em: 09/12/2012
% Criada por (author): Renato R. R. de Oliveira
% Contato: renatorro@comp.ufla.br
%
%
% Vers�o: 1.0 (20121209)
%

% Identifica��o da Classe
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{grubi-presentation}[2012/12/09]

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
\typeout{* - Ao usar imagens bitmaps, prefira o formato PNG, ou ent�o      }
\typeout{*   JPEG                                                          }
\typeout{*******************************************************************}
\typeout{}
\stop
\fi

% Carregamento da classe beamer e pacotes necess�rios
\LoadClass{beamer}
\RequirePackage[latin1]{inputenc}
\RequirePackage[T1]{fontenc}
\RequirePackage{graphicx}
\RequirePackage{indentfirst}
\RequirePackage{pdfpages}

% Estilizando.
\usetheme{Madrid}
\usefonttheme{structurebold}
\usecolortheme{beaver}

% Informa��es do GRUBI
\institute{\textbf{GRUBI}}
\logo{\includegraphics[width=2.0cm]{grubi}}

% Estilo da bibliografia:
\bibliographystyle{sbc}
