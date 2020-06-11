# Override
## Troisième projet sécurité de l'École 42

Si Rainfall vous a paru facile, voila de quoi vous donner du fil a retordre. Override est la dernière ISO de challenge qui vous demandera de partir à la recherche de failles présents dans des binaires protégés, et reconstruire ces binaires en fonction de leur comportement. 

### Setup de la VM

Réseau Virtualbox NAT -> Redirection de port

- **Nom :** SSH
- **Protocole :** TCP
- **Ip hôte :** 127.0.0.1
- **Port hote :** 4242
- **Ip invitée :** 10.0.2.15 (ip donnée par virtualbox, peut être différente)
- **Port invitée :** 4242 (port ssh de l'iso)

#### SSH / SCP

	ssh -p 4242 levelXX@127.0.0.1
	scp -P 4242 levelXX@127.0.0.1:/path/on/the/vm ./path/on/my/machine

:warning: :warning: :warning: :warning: :warning: :warning: :warning: :warning: :warning: :warning:

### !!! SPOIL !!!
