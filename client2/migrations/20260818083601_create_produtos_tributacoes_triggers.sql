-- FUNCTION: public.fn_produtos_tributacoes_bu0()

-- DROP FUNCTION IF EXISTS public.fn_produtos_tributacoes_bu0();

CREATE OR REPLACE FUNCTION public.fn_produtos_tributacoes_bu0()
    RETURNS trigger
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE NOT LEAKPROOF
AS $BODY$
	BEGIN
		NEW.atualizado_em := CURRENT_TIMESTAMP;
		RETURN NEW;
	END;
$BODY$;

-- Trigger: produtos_tributacoes_bu0

-- DROP TRIGGER IF EXISTS produtos_tributacoes_bu0 ON public.produtos_tributacoes;

CREATE OR REPLACE TRIGGER produtos_tributacoes_bu0
    BEFORE UPDATE
    ON public.produtos_tributacoes
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_produtos_tributacoes_bu0();