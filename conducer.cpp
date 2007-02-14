#include <QtDebug>
#include "conducer.h"
#include "conducersegment.h"
#include "element.h"

bool Conducer::pen_and_brush_initialized = false;
QPen Conducer::conducer_pen = QPen();
QBrush Conducer::conducer_brush = QBrush();

/**
	Constructeur
	@param p1     Premiere Borne auquel le conducteur est lie
	@param p2     Seconde Borne auquel le conducteur est lie
	@param parent Element parent du conducteur (0 par defaut)
	@param scene  QGraphicsScene auquelle appartient le conducteur
*/
Conducer::Conducer(Terminal *p1, Terminal* p2, Element *parent, QGraphicsScene *scene) : QGraphicsPathItem(parent, scene) {
	// bornes que le conducteur relie
	terminal1 = p1;
	terminal2 = p2;
	// ajout du conducteur a la liste de conducteurs de chacune des deux bornes
	bool ajout_p1 = terminal1 -> addConducer(this);
	bool ajout_p2 = terminal2 -> addConducer(this);
	// en cas d'echec de l'ajout (conducteur deja existant notamment)
	if (!ajout_p1 || !ajout_p2) return;
	destroyed = false;
	modified_path = false;
	// attributs de dessin par defaut (communs a tous les conducteurs)
	if (!pen_and_brush_initialized) {
		conducer_pen.setJoinStyle(Qt::MiterJoin);
		conducer_pen.setCapStyle(Qt::SquareCap);
		conducer_pen.setColor(Qt::black);
		conducer_pen.setStyle(Qt::SolidLine);
		conducer_pen.setWidthF(1.0);
		conducer_brush.setColor(Qt::white);
		conducer_brush.setStyle(Qt::NoBrush);
		pen_and_brush_initialized = true;
	}
	// calcul du rendu du conducteur
	segments = NULL;
	priv_calculeConducer(terminal1 -> amarrageConducer(), terminal1 -> orientation(), terminal2 -> amarrageConducer(), terminal2 -> orientation());
	setFlags(QGraphicsItem::ItemIsSelectable);
}

/**
	Met a jour la representation graphique du conducteur.
	@param rect Rectangle a mettre a jour
*/
void Conducer::update(const QRectF &rect) {
	// utilise soit la fonction priv_modifieConducteur soit la fonction priv_calculeConducteur
	void (Conducer::* fonction_update) (const QPointF &, Terminal::Orientation, const QPointF &, Terminal::Orientation);
	fonction_update = (nbSegments() && modified_path) ? &Conducer::priv_modifieConducer : &Conducer::priv_calculeConducer;
	
	// appelle la bonne fonction pour calculer l'aspect du conducteur
	(this ->* fonction_update)(
		terminal1 -> amarrageConducer(), terminal1 -> orientation(),
		terminal2 -> amarrageConducer(), terminal2 -> orientation()
	);
	QGraphicsPathItem::update(rect);
}

/**
	Met a jour la representation graphique du conducteur en considerant que la borne b
	a pour position pos
	@param rect Rectangle a mettre a jour
	@param b Borne
	@param pos position de la borne b
*/
void Conducer::updateWithNewPos(const QRectF &rect, const Terminal *b, const QPointF &newpos) {
	QPointF p1, p2;
	if (b == terminal1) {
		p1 = newpos;
		p2 = terminal2 -> amarrageConducer();
	} else if (b == terminal2) {
		p1 = terminal1 -> amarrageConducer();
		p2 = newpos;
	} else {
		p1 = terminal1 -> amarrageConducer();
		p2 = terminal2 -> amarrageConducer();
	}
	if (nbSegments() && modified_path)
		priv_modifieConducer(p1, terminal1 -> orientation(), p2, terminal2 -> orientation());
	else
		priv_calculeConducer(p1, terminal1 -> orientation(), p2, terminal2 -> orientation());
	QGraphicsPathItem::update(rect);
}

/**
	Genere le QPainterPath a partir de la liste des points
*/
void Conducer::segmentsToPath() {
	// chemin qui sera dessine
	QPainterPath path;
	
	// s'il n'y a pa des segments, on arrete la
	if (segments == NULL) setPath(path);
	
	// demarre le chemin
	path.moveTo(segments -> firstPoint());
	
	// parcourt les segments pour dessiner le chemin
	ConducerSegment *segment = segments;
	while(segment -> hasNextSegment()) {
		path.lineTo(segment -> secondPoint());
		segment = segment -> nextSegment();
	}
	
	// termine le chemin
	path.lineTo(segment -> secondPoint());
	
	// affecte le chemin au conducteur
	setPath(path);
}

/**
	Gere les updates 
	@param p1 Coordonnees du point d'amarrage de la borne 1
	@param o1 Orientation de la borne 1
	@param p2 Coordonnees du point d'amarrage de la borne 2
	@param o2 Orientation de la borne 2
*/
void Conducer::priv_modifieConducer(const QPointF &p1, Terminal::Orientation, const QPointF &p2, Terminal::Orientation) {
	Q_ASSERT_X(nbSegments() > 1, "priv_modifieConducer", "pas de points a modifier");
	
	// recupere les dernieres coordonnees connues des bornes
	QPointF old_p1 = mapFromScene(terminal1 -> amarrageConducer());
	QPointF old_p2 = mapFromScene(terminal2 -> amarrageConducer());
	
	// recupere les coordonnees fournies des bornes
	QPointF new_p1 = mapFromScene(p1);
	QPointF new_p2 = mapFromScene(p2);
	
	// les distances horizontales et verticales entre les anciennes bornes
	// sont stockees dans orig_dist_2_terms_x et orig_dist_2_terms_y
	
	// calcule les distances horizontales et verticales entre les nouvelles bornes
	qreal new_dist_2_terminals_x = new_p2.x() - new_p1.x();
	qreal new_dist_2_terminals_y = new_p2.y() - new_p1.y();
	
	// en deduit les coefficients de "redimensionnement"
	qreal coeff_x = new_dist_2_terminals_x / orig_dist_2_terms_x;
	qreal coeff_y = new_dist_2_terminals_y / orig_dist_2_terms_y;
	
	// genere les nouveaux points
	int limite = moves_x.size() - 1;
	int coeff = type_trajet_x ? 1 : -1;
	
	QList<QPointF> points;
	points << (type_trajet_x ? new_p1 : new_p2);
	for (int i = 0 ; i < limite ; ++ i) {
		QPointF previous_point = points.last();
		points << QPointF (
			previous_point.x() + (moves_x.at(i) * coeff_x * coeff),
			previous_point.y() + (moves_y.at(i) * coeff_y * coeff)
		);
	}
	points << (type_trajet_x ? new_p2 : new_p1);
	pointsToSegments(points);
	segmentsToPath();
}

/**
	Calcule un trajet "par defaut" pour le conducteur
	@param p1 Coordonnees du point d'amarrage de la borne 1
	@param o1 Orientation de la borne 1
	@param p2 Coordonnees du point d'amarrage de la borne 2
	@param o2 Orientation de la borne 2
*/
void Conducer::priv_calculeConducer(const QPointF &p1, Terminal::Orientation o1, const QPointF &p2, Terminal::Orientation o2) {
	QPointF sp1, sp2, depart, newp1, newp2, arrivee, depart0, arrivee0;
	Terminal::Orientation ori_depart, ori_arrivee;
	
	// s'assure qu'il n'y a ni points
	QList<QPointF> points;
	
	type_trajet_x = p1.x() < p2.x();
	// mappe les points par rapport a la scene
	sp1 = mapFromScene(p1);
	sp2 = mapFromScene(p2);
	
	// prolonge les bornes
	newp1 = extendTerminal(sp1, o1);
	newp2 = extendTerminal(sp2, o2);
	
	// distingue le depart de l'arrivee : le trajet se fait toujours de gauche a droite (apres prolongation)
	if (newp1.x() <= newp2.x()) {
		depart      = newp1;
		arrivee     = newp2;
		depart0     = sp1;
		arrivee0    = sp2;
		ori_depart  = o1;
		ori_arrivee = o2;
	} else {
		depart      = newp2;
		arrivee     = newp1;
		depart0     = sp2;
		arrivee0    = sp1;
		ori_depart  = o2;
		ori_arrivee = o1;
	}
	
	// debut du trajet
	points << depart0;
	
	// prolongement de la borne de depart 
	points << depart;
	
	// commence le vrai trajet
	if (depart.y() < arrivee.y()) {
		// trajet descendant
		if ((ori_depart == Terminal::Nord && (ori_arrivee == Terminal::Sud || ori_arrivee == Terminal::Ouest)) || (ori_depart == Terminal::Est && ori_arrivee == Terminal::Ouest)) {
			// cas � 3 �
			qreal ligne_inter_x = (depart.x() + arrivee.x()) / 2.0;
			points << QPointF(ligne_inter_x, depart.y());
			points << QPointF(ligne_inter_x, arrivee.y());
		} else if ((ori_depart == Terminal::Sud && (ori_arrivee == Terminal::Nord || ori_arrivee == Terminal::Est)) || (ori_depart == Terminal::Ouest && ori_arrivee == Terminal::Est)) {
			// cas � 4 �
			qreal ligne_inter_y = (depart.y() + arrivee.y()) / 2.0;
			points << QPointF(depart.x(), ligne_inter_y);
			points << QPointF(arrivee.x(), ligne_inter_y);
		} else if ((ori_depart == Terminal::Nord || ori_depart == Terminal::Est) && (ori_arrivee == Terminal::Nord || ori_arrivee == Terminal::Est)) {
			points << QPointF(arrivee.x(), depart.y()); // cas � 2 �
		} else {
			points << QPointF(depart.x(), arrivee.y()); // cas � 1 �
		}
	} else {
		// trajet montant
		if ((ori_depart == Terminal::Ouest && (ori_arrivee == Terminal::Est || ori_arrivee == Terminal::Sud)) || (ori_depart == Terminal::Nord && ori_arrivee == Terminal::Sud)) {
			// cas � 3 �
			qreal ligne_inter_y = (depart.y() + arrivee.y()) / 2.0;
			points << QPointF(depart.x(), ligne_inter_y);
			points << QPointF(arrivee.x(), ligne_inter_y);
		} else if ((ori_depart == Terminal::Est && (ori_arrivee == Terminal::Ouest || ori_arrivee == Terminal::Nord)) || (ori_depart == Terminal::Sud && ori_arrivee == Terminal::Nord)) {
			// cas � 4 �
			qreal ligne_inter_x = (depart.x() + arrivee.x()) / 2.0;
			points << QPointF(ligne_inter_x, depart.y());
			points << QPointF(ligne_inter_x, arrivee.y());
		} else if ((ori_depart == Terminal::Ouest || ori_depart == Terminal::Nord) && (ori_arrivee == Terminal::Ouest || ori_arrivee == Terminal::Nord)) {
			points << QPointF(depart.x(), arrivee.y()); // cas � 2 �
		} else {
			points << QPointF(arrivee.x(), depart.y()); // cas � 1 �
		}
	}
	
	// fin du vrai trajet
	points << arrivee;
	
	// prolongement de la borne d'arrivee
	points << arrivee0;
	
	pointsToSegments(points);
	segmentsToPath();
}

/**
	Prolonge une borne.
	@param terminal Le point correspondant a la borne
	@param terminal_orientation L'orientation de la borne
	@param ext_size la taille de la prolongation
	@return le point correspondant a la borne apres prolongation
*/
QPointF Conducer::extendTerminal(const QPointF &terminal, Terminal::Orientation terminal_orientation, qreal ext_size) {
	QPointF extended_terminal;
	switch(terminal_orientation) {
		case Terminal::Nord:
			extended_terminal = QPointF(terminal.x(), terminal.y() - ext_size);
			break;
		case Terminal::Est:
			extended_terminal = QPointF(terminal.x() + ext_size, terminal.y());
			break;
		case Terminal::Sud:
			extended_terminal = QPointF(terminal.x(), terminal.y() + ext_size);
			break;
		case Terminal::Ouest:
			extended_terminal = QPointF(terminal.x() - ext_size, terminal.y());
			break;
		default: extended_terminal = terminal;
	}
	return(extended_terminal);
}

/**
	Dessine le conducteur sans antialiasing.
	@param qp Le QPainter a utiliser pour dessiner le conducteur
	@param qsogi Les options de style pour le conducteur
	@param qw Le QWidget sur lequel on dessine 
*/
void Conducer::paint(QPainter *qp, const QStyleOptionGraphicsItem */*qsogi*/, QWidget */*qw*/) {
	qp -> save();
	qp -> setRenderHint(QPainter::Antialiasing, false);
	
	// affectation du QPen et de la QBrush modifies au QPainter
	qp -> setBrush(conducer_brush);
	qp -> setPen(conducer_pen);
	if (isSelected()) {
		QPen tmp = qp -> pen();
		tmp.setColor(Qt::red);
		qp -> setPen(tmp);
	}
	
	// dessin du conducteur
	qp -> drawPath(path());
	
	// dessin des points d'accroche du conducteur si celui-ci est selectionne
	if (isSelected()) {
		qp -> setRenderHint(QPainter::Antialiasing, true);
		QList<QPointF> points = segmentsToPoints();
		QPointF previous_point;
		QBrush square_brush(Qt::darkGreen);
		for (int i = 1 ; i < (points.size() -1) ; ++ i) {
			QPointF point = points.at(i);
			
			if (i > 1) {
				qp -> fillRect(
					QRectF(
						((previous_point.x() + point.x()) / 2.0 ) - 2.5,
						((previous_point.y() + point.y()) / 2.0 ) - 2.5,
						5.0,
						5.0
					),
					square_brush
				);
			}
			qp -> drawEllipse(QRectF(point.x() - 3.0, point.y() - 3.0, 6.0, 6.0));
			previous_point = point;
		}
	}
	qp -> restore();
}

/**
	Indique si deux orientations de Borne sont sur le meme axe (Vertical / Horizontal).
	@param a La premiere orientation de Borne
	@param b La seconde orientation de Borne
	@return Un booleen a true si les deux orientations de bornes sont sur le meme axe
*/
bool Conducer::surLeMemeAxe(Terminal::Orientation a, Terminal::Orientation b) {
	if ((a == Terminal::Nord || a == Terminal::Sud) && (b == Terminal::Nord || b == Terminal::Sud)) return(true);
	else if ((a == Terminal::Est || a == Terminal::Ouest) && (b == Terminal::Est || b == Terminal::Ouest)) return(true);
	else return(false);
}

/**
	Indique si une orientation de borne est horizontale (Est / Ouest).
	@param a L'orientation de borne
	@return True si l'orientation de borne est horizontale, false sinon
*/
bool Conducer::estHorizontale(Terminal::Orientation a) {
	return(a == Terminal::Est || a == Terminal::Ouest);
}

/**
	Indique si une orientation de borne est verticale (Nord / Sud).
	@param a L'orientation de borne
	@return True si l'orientation de borne est verticale, false sinon
*/
bool Conducer::estVerticale(Terminal::Orientation a) {
	return(a == Terminal::Nord || a == Terminal::Sud);
}

/**
	Methode de preparation a la destruction du conducteur ; le conducteur se detache de ses deux bornes
*/
void Conducer::destroy() {
	destroyed = true;
	terminal1 -> removeConducer(this);
	terminal2 -> removeConducer(this);
}

/**
	Methode de validation d'element XML
	@param e Un element XML sense represente un Conducteur
	@return true si l'element XML represente bien un Conducteur ; false sinon
*/
bool Conducer::valideXml(QDomElement &e){
	// verifie le nom du tag
	if (e.tagName() != "conducteur") return(false);
	
	// verifie la presence des attributs minimaux
	if (!e.hasAttribute("borne1")) return(false);
	if (!e.hasAttribute("borne2")) return(false);
	
	bool conv_ok;
	// parse l'abscisse
	e.attribute("borne1").toInt(&conv_ok);
	if (!conv_ok) return(false);
	
	// parse l'ordonnee
	e.attribute("borne2").toInt(&conv_ok);
	if (!conv_ok) return(false);
	return(true);
}

/**
	Gere les clics sur le conducteur.
	@param e L'evenement decrivant le clic.
*/
void Conducer::mousePressEvent(QGraphicsSceneMouseEvent *e) {
	// clic gauche
	if (e -> buttons() & Qt::LeftButton) {
		// recupere les coordonnees du clic
		press_point = mapFromScene(e -> pos());
		
		/*
			parcourt les segments pour determiner si le clic a eu lieu
			- sur l'extremite d'un segment
			- sur le milieu d'un segment
			- ailleurs
		*/
		ConducerSegment *segment = segments;
		while (segment -> hasNextSegment()) {
			if (hasClickedOn(press_point, segment -> secondPoint())) {
				moving_point = true;
				moving_segment = false;
				moved_segment = segment;
				break;
			} else if (hasClickedOn(press_point, segment -> middle())) {
				moving_point = false;
				moving_segment = true;
				moved_segment = segment;
				break;
			}
			segment = segment -> nextSegment();
		}
	}
	QGraphicsPathItem::mousePressEvent(e);
}

/**
	Gere les deplacements de souris sur le conducteur.
	@param e L'evenement decrivant le deplacement de souris.
	@todo
	-calculer le trajet du conducteur differemment selon l'etat du flag "trajet modifie"
	-garder une liste des points constituants le trajet
	-lorsque le fil est selectionne, dessiner ces points (cercles)
	-lors d'un mousemoveevent: detecter la position du clic : si cela tombe dans la zone d'un point :
		-deplacer ce point en consequence
		-mettre le flag "trajet modifie" a true
	-gerer les contraintes
*/
void Conducer::mouseMoveEvent(QGraphicsSceneMouseEvent *e) {
	// clic gauche
	if (e -> buttons() & Qt::LeftButton) {
		// position pointee par la souris
		qreal mouse_x = e -> pos().x();
		qreal mouse_y = e -> pos().y();
		
		if (moving_point) {
			// la modification par points revient bientot
			/*
			// position precedente du point
			QPointF p = moved_segment -> secondPoint();
			qreal p_x = p.x();
			qreal p_y = p.y();
			
			// calcul du deplacement
			moved_segment -> moveX(mouse_x - p_x());
			moved_segment -> moveY(mouse_y - p_y());
			
			// application du deplacement
			modified_path = true;
			updatePoints();
			segmentsToPath();
			*/
		} else if (moving_segment) {
			// position precedente du point
			QPointF p = moved_segment -> middle();
			
			// calcul du deplacement
			moved_segment -> moveX(mouse_x - p.x());
			moved_segment -> moveY(mouse_y - p.y());
			
			// application du deplacement
			modified_path = true;
			updatePoints();
			segmentsToPath();
		}
	}
	QGraphicsPathItem::mouseMoveEvent(e);
}

/**
	Gere les relachements de boutons de souris sur le conducteur 
	@param e L'evenement decrivant le lacher de bouton.
*/
void Conducer::mouseReleaseEvent(QGraphicsSceneMouseEvent *e) {
	// clic gauche
	moving_point = false;
	moving_segment = false;
	QGraphicsPathItem::mouseReleaseEvent(e);
}

/**
	@return Le rectangle delimitant l'espace de dessin du conducteur
*/
QRectF Conducer::boundingRect() const {
	QRectF retour = QGraphicsPathItem::boundingRect();
	retour.adjust(-5.0, -5.0, 5.0, 5.0);
	return(retour);
}

/**
	@return La forme / zone "cliquable" du conducteur
*/
QPainterPath Conducer::shape() const {
	QList<QPointF> points = segmentsToPoints();
	QPainterPath area;
	QPointF previous_point;
	QPointF *point1, *point2;
	foreach(QPointF point, points) {
		if (!previous_point.isNull()) {
			if (point.x() == previous_point.x()) {
				if (point.y() <= previous_point.y()) {
					point1 = &point;
					point2 = &previous_point;
				} else {
					point1 = &previous_point;
					point2 = &point;
				}
			} else {
				if (point.x() <= previous_point.x()) {
					point1 = &point;
					point2 = &previous_point;
				} else {
					point1 = &previous_point;
					point2 = &point;
				}
			}
			qreal p1_x = point1 -> x();
			qreal p1_y = point1 -> y();
			qreal p2_x = point2 -> x();
			qreal p2_y = point2 -> y();
			area.setFillRule(Qt::OddEvenFill);
			area.addRect(p1_x - 5.0, p1_y - 5.0, 10.0 + p2_x - p1_x, 10.0 + p2_y - p1_y);
		}
		previous_point = point;
		area.setFillRule(Qt::WindingFill);
		area.addRect(point.x() - 5.0, point.y() - 5.0, 10.0, 10.0);
	}
	return(area);
}

/**
	Met � jour deux listes de reels.
*/
void Conducer::updatePoints() {
	QList<QPointF> points = segmentsToPoints();
	int s = points.size();
	moves_x.clear();
	moves_y.clear();
	for (int i = 1 ; i < s ; ++ i) {
		moves_x << points.at(i).x() - points.at(i - 1).x();
		moves_y << points.at(i).y() - points.at(i - 1).y();
	}
	QPointF b1 = points.at(0);
	QPointF b2 = points.at(s - 1);
	orig_dist_2_terms_x = b2.x() - b1.x();
	orig_dist_2_terms_y = b2.y() - b1.y();
}

qreal Conducer::conducer_bound(qreal tobound, qreal bound1, qreal bound2) {
	qreal space = 5.0;
	if (bound1 < bound2) {
		return(qBound(bound1 + space, tobound, bound2 - space));
	} else {
		return(qBound(bound2 + space, tobound, bound1 - space));
	}
}

qreal Conducer::conducer_bound(qreal tobound, qreal bound, bool positive) {
	qreal space = 5.0;
	return(positive ? qMax(tobound, bound + space) : qMin(tobound, bound - space));
}

int Conducer::nbSegments() {
	if (segments == NULL) return(0);
	int nb_seg = 1;
	ConducerSegment *segment = segments;
	while (segment -> hasNextSegment()) {
		++ nb_seg;
		segment = segment -> nextSegment();
	}
	return(nb_seg);
}

/**
	Genere une liste de points a partir des segments de ce conducteur
	@return La liste de points representant ce conducteur
*/
QList<QPointF> Conducer::segmentsToPoints() const {
	// liste qui sera retournee
	QList<QPointF> points_list;
	
	// on retourne la liste tout de suite s'il n'y a pas de segments
	if (segments == NULL) return(points_list);
	
	// recupere le premier point
	points_list << segments -> firstPoint();
	
	// parcourt les segments pour recuperer les autres points
	ConducerSegment *segment = segments;
	while(segment -> hasNextSegment()) {
		points_list << segment -> secondPoint();
		segment = segment -> nextSegment();
	}
	
	// recupere le dernier point
	points_list << segment -> secondPoint();
	
	//retourne la liste
	return(points_list);
}

/**
	Regenere les segments de ce conducteur a partir de la liste de points passee en parametre
	@param points_list Liste de points a utiliser pour generer les segments
*/
void Conducer::pointsToSegments(QList<QPointF> points_list) {
	// supprime les segments actuels
	if (segments != NULL) {
		ConducerSegment *segment = segments;
		while (segment -> hasNextSegment()) {
			ConducerSegment *nextsegment = segment -> nextSegment();
			delete segment;
			segment = nextsegment;
		}
	}
	
	// cree les segments a partir de la liste de points
	ConducerSegment *last_segment = NULL;
	for (int i = 0 ; i < points_list.size() - 1 ; ++ i) {
		last_segment = new ConducerSegment(points_list.at(i), points_list.at(i + 1), last_segment);
		if (!i) segments = last_segment;
	}
}

bool Conducer::hasClickedOn(QPointF press_point, QPointF point) {
	return (
		press_point.x() >= point.x() - 5.0 &&\
		press_point.x() <  point.x() + 5.0 &&\
		press_point.y() >= point.y() - 5.0 &&\
		press_point.y() <  point.y() + 5.0
	);
}
