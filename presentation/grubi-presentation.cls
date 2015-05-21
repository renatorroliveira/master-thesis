% Classe GRUBI Presentations - Apresetações do Grupo de Redes Ubíquas da UFLA
%
% Encoding do Arquivo: Western European ISO-8859-1 (latin1)
% Criada em: 09/12/2012
% Criada por (author): Renato R. R. de Oliveira
% Contato: renatorro@comp.ufla.br
%
%
% Versão: 1.0 (20121209)
%

% Identificação da Classe
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{grubi-presentation}[2012/12/09]

% Compilacao preferencial em pdfLaTeX
\ifx\pdfoutput\undefined
\typeout{}
\typeout{*******************************************************************}
\typeout{* AVISO: Essa classe não foi feita para ser usada com LaTeX puro! }
\typeout{* Use pdfLaTeX!                                                   }
\typeout{*                                                                 }
\typeout{* Se você antes utilizava o comando                               }
\typeout{* latex arquivo; dvips arquivo                                    }
\typeout{* Então basta agora digitar                                       }
\typeout{* pdflatex arquivo                                                }
\typeout{*                                                                 }
\typeout{* Dicas:                                                          }
\typeout{* - Ao usar imagens vetoriais, prefira exportar para pdf direto   }
\typeout{*   ou então exporte para eps e utilize o comando                 }
\typeout{*   epstopdf arquivo.eps                                          }
\typeout{*   para gerar um pdf a partir de um eps                          }
\typeout{* - Ao usar imagens bitmaps, prefira o formato PNG, ou então      }
\typeout{*   JPEG                                                          }
\typeout{*******************************************************************}
\typeout{}
\stop
\fi

% Carregamento da classe beamer e pacotes necessários
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

% Informações do GRUBI
\institute{\textbf{GRUBI}}
\logo{\includegraphics[width=2.0cm]{grubi}}

% Estilo da bibliografia:
\bibliographystyle{sbc}
