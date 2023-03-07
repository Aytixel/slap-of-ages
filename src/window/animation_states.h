/**
 * @brief   Enumération des états d'une animation de portail
 * @details Les états sont les suivants :
 *          - PORTAL_IDLE : le portail est ouvert
 *          - PORTAL_SPAWN : le portail apparait
 *          - PORTAL_DESPAWN : le portail disparait
 * @param PORTAL_IDLE
 * @param PORTAL_SPAWN
 * @param PORTAL_DESPAWN
 * @return enum
 */

typedef enum
{
    PORTAL_IDLE,
    PORTAL_SPAWN,
    PORTAL_DESPAWN,

} portal_e;

/**
 * @brief   Enumération des états d'une animation de livre
 * @details Les états sont les suivants :
 *          - BOOK_OPEN : le livre s'ouvre
 *          - BOOK_CLOSE : le livre se ferme
 * @param BOOK_OPEN
 * @param BOOK_CLOSE
 * @return enum
 */

typedef enum
{
    BOOK_OPEN,
    BOOK_CLOSE,

} book_e;